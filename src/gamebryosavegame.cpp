#include "gamebryosavegame.h"

#include "iplugingame.h"
#include "scriptextender.h"

#include <QDate>
#include <QFile>
#include <QFileInfo>
#include <QScopedArrayPointer>
#include <QTime>

#include <Windows.h>
#include <lz4.h>

#include <stdexcept>
#include <vector>

GamebryoSaveGame::GamebryoSaveGame(QString const &file, MOBase::IPluginGame const *game, bool const lightEnabled) :
  m_FileName(file),
  m_CreationTime(QFileInfo(file).lastModified()),
  m_Game(game),
  m_LightEnabled(lightEnabled)
{
}

GamebryoSaveGame::~GamebryoSaveGame()
{
}

QString GamebryoSaveGame::getFilename() const
{
  return m_FileName;
}

QDateTime GamebryoSaveGame::getCreationTime() const
{
  return m_CreationTime;
}

QString GamebryoSaveGame::getSaveGroupIdentifier() const
{
  return m_PCName;
}

QStringList GamebryoSaveGame::allFiles() const
{
  //This returns all valid files associated with this game
  QStringList res = { m_FileName };
  ScriptExtender const *e = m_Game->feature<ScriptExtender>();
  if (e != nullptr) {
    QFileInfo file(m_FileName);
    for (QString const &ext : e->saveGameAttachmentExtensions()) {
      QFileInfo name(file.absoluteDir().absoluteFilePath(file.completeBaseName() + "." + ext));
      if (name.exists()) {
        res.push_back(name.absoluteFilePath());
      }
    }

    QFileInfo SEfile(file.absolutePath() + "/" + file.completeBaseName() + "." + m_Game->savegameSEExtension());
    if (SEfile.exists()) {
      res.push_back(SEfile.absoluteFilePath());
    }
  }
  return res;
}

bool GamebryoSaveGame::hasScriptExtenderFile() const
{
  QFileInfo file(m_FileName);
  QFileInfo SEfile(file.absolutePath() + "/" + file.completeBaseName() + "." + m_Game->savegameSEExtension());
  return SEfile.exists();
}

void GamebryoSaveGame::setCreationTime(_SYSTEMTIME const &ctime)
{
  QDate date;
  date.setDate(ctime.wYear, ctime.wMonth, ctime.wDay);
  QTime time;
  time.setHMS(ctime.wHour, ctime.wMinute, ctime.wSecond, ctime.wMilliseconds);

  m_CreationTime = QDateTime(date, time, Qt::UTC);
}

GamebryoSaveGame::FileWrapper::FileWrapper(GamebryoSaveGame *game,
                                           QString const &expected) :
  m_Game(game),
  m_File(game->m_FileName),
  m_HasFieldMarkers(false),
  m_PluginString(StringType::TYPE_WSTRING)
{
  if (!m_File.open(QIODevice::ReadOnly)) {
    throw std::runtime_error(QObject::tr("failed to open %1").arg(game->m_FileName).toUtf8().constData());
  }

  std::vector<char> fileID(expected.length() + 1);
  m_File.read(fileID.data(), expected.length());
  fileID[expected.length()] = '\0';

  QString id(fileID.data());
  if (expected != id) {
    throw std::runtime_error(
      QObject::tr("wrong file format - expected %1 got %2").arg(expected).arg(id).toUtf8().constData());
  }
}

void GamebryoSaveGame::FileWrapper::setHasFieldMarkers(bool state)
{
  m_HasFieldMarkers = state;
}

void GamebryoSaveGame::FileWrapper::setPluginString(StringType type)
{
  m_PluginString = type;
}

template <> void GamebryoSaveGame::FileWrapper::read(QString &value)
{
  unsigned short length;
  if (m_PluginString == StringType::TYPE_BSTRING || m_PluginString == StringType::TYPE_BZSTRING) {
    unsigned char len;
    read(len);
    length = m_PluginString == StringType::TYPE_BZSTRING ? len + 1 : len;
  } else {
    read(length);
  }

  if (m_HasFieldMarkers) {
    skip<char>();
  }

  char *buffer = new char[length];

  read(buffer, m_PluginString == StringType::TYPE_BZSTRING ? length - 1 : length);

  if (m_PluginString == StringType::TYPE_BZSTRING)
    buffer[length - 1] = '\0';

  if (m_HasFieldMarkers) {
    skip<char>();
  }

  value = QString::fromLatin1(buffer, length);

  delete buffer;
}

void GamebryoSaveGame::FileWrapper::read(void *buff, std::size_t length)
{
  int read = m_File.read(static_cast<char *>(buff), length);
  if (read != length) {
    throw std::runtime_error("unexpected end of file");
  }
}

void GamebryoSaveGame::FileWrapper::readImage(int scale, bool alpha)
{
  unsigned long width;
  read(width);
  unsigned long height;
  read(height);
  readImage(width, height, scale, alpha);
}

void GamebryoSaveGame::FileWrapper::readImage(unsigned long width, unsigned long height, int scale, bool alpha)
{
  int bpp = alpha ? 4 : 3;
  QScopedArrayPointer<unsigned char> buffer(new unsigned char[width * height * bpp]);
  read(buffer.data(), width * height * bpp);
  QImage image(buffer.data(), width, height, alpha ? QImage::Format_RGBA8888_Premultiplied
    : QImage::Format_RGB888);
  if (scale != 0) {
    m_Game->m_Screenshot = image.copy().scaledToWidth(scale);
  } else {
    // why do I have to copy here? without the copy, the buffer seems to get
    // deleted after the temporary vanishes, but shouldn't Qts implicit sharing
    // handle that?
    m_Game->m_Screenshot = image.copy();
  }
}
void readQDataStream(QDataStream &data, void *buff, std::size_t length) {
  int read = data.readRawData(static_cast<char *>(buff), static_cast<int>(length));
  if (read != length) {
    throw std::runtime_error("unexpected end of file");
  }
}
template <typename T> void readQDataStream(QDataStream &data, T &value) {
  int read = data.readRawData(reinterpret_cast<char*>(&value), sizeof(T));
  if (read != sizeof(T)) {
    throw std::runtime_error("unexpected end of file");
  }
}

template <> void readQDataStream(QDataStream &data, QString &value)
{
  unsigned short length;
  readQDataStream(data, length);

  std::vector<char> buffer(length);

  readQDataStream(data, buffer.data(), length);

  value = QString::fromLatin1(buffer.data(), length);
}

void GamebryoSaveGame::FileWrapper::closeCompressedData()
{
  if (m_Game->m_CompressionType == 0) {
  }
  else if (m_Game->m_CompressionType == 1) {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found zlib Compressed\" with your savefile attached");
  }
  else if (m_Game->m_CompressionType == 2) {
    m_Data->device()->close();
    delete m_Data;
  }
  else
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
}

bool GamebryoSaveGame::FileWrapper::openCompressedData(int bytesToIgnore)
{
  if (m_Game->m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    return false;
  } else if (m_Game->m_CompressionType == 1) {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found zlib Compressed\" with your savefile attached");
    return false;
  } else if (m_Game->m_CompressionType == 2) {
    uint32_t uncompressedSize;
    read(uncompressedSize);
    uint32_t compressedSize;
    read(compressedSize);
    QByteArray compressed;
    compressed.resize(compressedSize);
    read(compressed.data(), compressedSize);
    QByteArray decompressed;
    decompressed.resize(uncompressedSize);
    LZ4_decompress_safe_partial(compressed.data(), decompressed.data(), compressedSize, uncompressedSize, uncompressedSize);
    compressed.clear();

    m_Data = new QDataStream(decompressed);
    m_Data->skipRawData(bytesToIgnore);

    return true;
  } else {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return false;
  }
}

uint8_t GamebryoSaveGame::FileWrapper::readChar(int bytesToIgnore)
{
  if (m_Game->m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint8_t version;
    read(version);
    return version;
  } else if (m_Game->m_CompressionType == 1) {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found zlib Compressed\" with your savefile attached");
    return 0;
  } else if (m_Game->m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    uint8_t version;
    readQDataStream(*m_Data, version);
    return version;

  } else {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

uint16_t GamebryoSaveGame::FileWrapper::readShort(int bytesToIgnore)
{
  if (m_Game->m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint16_t size;
    read(size);
    return size;
  } else if (m_Game->m_CompressionType == 1) {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found zlib Compressed\" with your savefile attached");
    return 0;
  } else if (m_Game->m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    uint16_t size;
    readQDataStream(*m_Data, size);
    return size;
  } else {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

uint32_t GamebryoSaveGame::FileWrapper::readInt(int bytesToIgnore)
{
  if (m_Game->m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint32_t size;
    read(size);
    return size;
  } else if (m_Game->m_CompressionType == 1) {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found zlib Compressed\" with your savefile attached");
    return 0;
  } else if (m_Game->m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    uint32_t size;
    readQDataStream(*m_Data, size);
    return size;
  } else {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

void GamebryoSaveGame::FileWrapper::readPlugins(int bytesToIgnore)
{
  if (m_Game->m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint8_t count;
    read(count);
    uint16_t finalCount = count;
    m_Game->m_Plugins.reserve(finalCount);
    for (std::size_t i = 0; i < finalCount; ++i) {
      QString name;
      read(name);
      m_Game->m_Plugins.push_back(name);
    }
  } else if (m_Game->m_CompressionType == 1) {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found zlib Compressed\" with your savefile attached");
  } else if (m_Game->m_CompressionType == 2) {
    m_Data->skipRawData(bytesToIgnore);
    uint8_t count;
    readQDataStream(*m_Data, count);
    uint16_t finalCount = count;
    m_Game->m_Plugins.reserve(finalCount);
    for (std::size_t i = 0; i<finalCount; ++i) {
      QString name;
      readQDataStream(*m_Data, name);
      m_Game->m_Plugins.push_back(name);
    }
  }
}

void GamebryoSaveGame::FileWrapper::readLightPlugins(int bytesToIgnore)
{
  if (m_Game->m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint16_t count;
    read(count);
    m_Game->m_LightPlugins.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
      QString name;
      read(name);
      m_Game->m_LightPlugins.push_back(name);
    }
  } else if (m_Game->m_CompressionType == 1) {
    m_Game->m_Plugins.push_back("Please create an issue on the MO github labeled \"Found zlib Compressed\" with your savefile attached");
  } else if (m_Game->m_CompressionType == 2) {
    m_Data->skipRawData(bytesToIgnore);

    uint16_t count;
    readQDataStream(*m_Data, count);
    m_Game->m_LightPlugins.reserve(count);
    for (std::size_t i = 0; i<count; ++i) {
      QString name;
      readQDataStream(*m_Data, name);
      m_Game->m_LightPlugins.push_back(name);
    }

  }
}

void GamebryoSaveGame::FileWrapper::close()
{
  m_File.close();
}