#include "gamebryosavegame.h"

#include "iplugingame.h"
#include "scriptextender.h"
#include "log.h"

#include <QDate>
#include <QFile>
#include <QFileInfo>
#include <QScopedArrayPointer>
#include <QTime>

#include <Windows.h>
#include <lz4.h>
#include <zlib.h>

#include <stdexcept>
#include <vector>


#include "gamegamebryo.h"

#define CHUNK 16384

GamebryoSaveGame::GamebryoSaveGame(QString const &file, GameGamebryo const *game, bool const lightEnabled) :
  m_FileName(file),
  m_CreationTime(QFileInfo(file).lastModified()),
  m_Game(game),
  m_LightEnabled(lightEnabled),
  m_DataFields([this]() { return fetchDataFields(); })
{
}

GamebryoSaveGame::~GamebryoSaveGame()
{
}

QString GamebryoSaveGame::getFilepath() const
{
  return m_FileName;
}

QDateTime GamebryoSaveGame::getCreationTime() const
{
  return m_CreationTime;
}

QString GamebryoSaveGame::getName() const
{
  return QObject::tr("%1, #%2, Level %3, %4")
    .arg(m_PCName)
    .arg(m_SaveNumber)
    .arg(m_PCLevel)
    .arg(m_PCLocation);
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

GamebryoSaveGame::FileWrapper::FileWrapper(QString const& filepath, QString const &expected) :
  m_File(filepath),
  m_HasFieldMarkers(false),
  m_PluginString(StringType::TYPE_WSTRING)
{
  if (!m_File.open(QIODevice::ReadOnly)) {
    throw std::runtime_error(QObject::tr("failed to open %1").arg(filepath).toUtf8().constData());
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

void readQDataStream(QDataStream& data, void* buff, std::size_t length) {
  int read = data.readRawData(static_cast<char*>(buff), static_cast<int>(length));
  if (read != length) {
    throw std::runtime_error("unexpected end of file");
  }
}

template <typename T> void readQDataStream(QDataStream& data, T& value) {
  int read = data.readRawData(reinterpret_cast<char*>(&value), sizeof(T));
  if (read != sizeof(T)) {
    throw std::runtime_error("unexpected end of file");
  }
}

template <> void readQDataStream(QDataStream& data, QString& value)
{
  unsigned short length;
  readQDataStream(data, length);

  std::vector<char> buffer(length);

  readQDataStream(data, buffer.data(), length);

  value = QString::fromLatin1(buffer.data(), length);
}

template <> void GamebryoSaveGame::FileWrapper::read(QString &value)
{
  if (m_CompressionType == 0) {
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

    QByteArray buffer;
    buffer.resize(length);

    read(buffer.data(), m_PluginString == StringType::TYPE_BZSTRING ? length - 1 : length);

    if (m_PluginString == StringType::TYPE_BZSTRING)
      buffer[length - 1] = '\0';

    if (m_HasFieldMarkers) {
      skip<char>();
    }

    value = QString::fromUtf8(buffer.constData());
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    readQDataStream(*m_Data, value);
  } else {
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
  }
}

void GamebryoSaveGame::FileWrapper::read(void *buff, std::size_t length)
{
  if (m_CompressionType == 0) {
    int read = m_File.read(static_cast<char*>(buff), length);
    if (read != length) {
      throw std::runtime_error("unexpected end of file");
    }
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    readQDataStream(*m_Data, buff, length);
  }  else {
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
  }
}

QImage GamebryoSaveGame::FileWrapper::readImage(int scale, bool alpha)
{
  unsigned long width;
  read(width);
  unsigned long height;
  read(height);
  return readImage(width, height, scale, alpha);
}

QImage GamebryoSaveGame::FileWrapper::readImage(unsigned long width, unsigned long height, int scale, bool alpha)
{
  int bpp = alpha ? 4 : 3;
  QScopedArrayPointer<unsigned char> buffer(new unsigned char[width * height * bpp]);
  read(buffer.data(), width * height * bpp);
  QImage image(buffer.data(), width, height, alpha ? QImage::Format_RGBA8888_Premultiplied
    : QImage::Format_RGB888);

  // We need to copy the image here because QImage does not make a copy of the
  // buffer when constructed.
  if (scale != 0) {
    return image.copy().scaledToWidth(scale);
  } else {
    return image.copy();
  }
}

void GamebryoSaveGame::FileWrapper::setCompressionType(uint16_t compressionType)
{
  m_CompressionType = compressionType;
}

void GamebryoSaveGame::FileWrapper::closeCompressedData()
{
  if (m_CompressionType == 0) {
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    m_Data->device()->close();
    delete m_Data;
  }
  else
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
}

bool GamebryoSaveGame::FileWrapper::openCompressedData(int bytesToIgnore)
{
  if (m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    return false;
  } else if (m_CompressionType == 1) {
    uint64_t location;
    read(location);
    uint64_t uncompressedSize;
    read(uncompressedSize);
    seek(location);
    uInt have;
    uInt size = 0;
    std::unique_ptr<unsigned char[]> inBuffer(new unsigned char[CHUNK]);
    std::unique_ptr<unsigned char[]> outBuffer(new unsigned char[CHUNK]);
    QByteArray finalData;
    z_stream stream;
    try {
      stream.zalloc = Z_NULL;
      stream.zfree = Z_NULL;
      stream.opaque = Z_NULL;
      stream.avail_in = 0;
      stream.next_in = Z_NULL;
      int zlibRet = inflateInit2(&stream, 15 + 32);
      if (zlibRet != Z_OK) {
        return false;
      }
      do {
        stream.avail_in = m_File.read(reinterpret_cast<char*>(inBuffer.get()), CHUNK);
        if (!m_File.isReadable()) {
          (void)inflateEnd(&stream);
          return false;
        }
        if (stream.avail_in == 0)
          break;
        stream.next_in = static_cast<Bytef*>(inBuffer.get());
        do {
          stream.avail_out = CHUNK;
          stream.next_out = reinterpret_cast<Bytef*>(outBuffer.get());
          zlibRet = inflate(&stream, Z_NO_FLUSH);
          if ((zlibRet != Z_OK) && (zlibRet != Z_STREAM_END) && (zlibRet != Z_BUF_ERROR)) {
            return false;
          }
          have = CHUNK - stream.avail_out;
          size += have;
          finalData += QByteArray::fromRawData(reinterpret_cast<const char*>(outBuffer.get()), have);
        } while (stream.avail_out == 0);
      } while (zlibRet != Z_STREAM_END);
      inflateEnd(&stream);
    } catch (const std::exception&) {
      inflateEnd(&stream);
      return false;
    }
    m_Data = new QDataStream(finalData);
    m_Data->skipRawData(bytesToIgnore);
    return true;
  } else if (m_CompressionType == 2) {
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
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return false;
  }
}

uint8_t GamebryoSaveGame::FileWrapper::readChar(int bytesToIgnore)
{
  if (m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint8_t version;
    read(version);
    return version;
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    uint8_t version;
    readQDataStream(*m_Data, version);
    return version;

  } else {
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

uint16_t GamebryoSaveGame::FileWrapper::readShort(int bytesToIgnore)
{
  if (m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint16_t size;
    read(size);
    return size;
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    uint16_t size;
    readQDataStream(*m_Data, size);
    return size;
  } else {
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

uint32_t GamebryoSaveGame::FileWrapper::readInt(int bytesToIgnore)
{
  if (m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint32_t size;
    read(size);
    return size;
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    uint32_t size;
    readQDataStream(*m_Data, size);
    return size;
  } else {
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

uint64_t GamebryoSaveGame::FileWrapper::readLong(int bytesToIgnore)
{
  if (m_CompressionType == 0) {
    if (bytesToIgnore > 0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint64_t size;
    read(size);
    return size;
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    uint64_t size;
    readQDataStream(*m_Data, size);
    return size;
  } else {
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

float_t GamebryoSaveGame::FileWrapper::readFloat(int bytesToIgnore)
{
  if (m_CompressionType == 0) {
    if (bytesToIgnore > 0)//Just to make certain
      skip<char>(bytesToIgnore);
    float_t value;
    read(value);
    return value;
  }
  else if (m_CompressionType == 1 || m_CompressionType == 2) {
    // decompression already done by readSaveGameVersion
    m_Data->skipRawData(bytesToIgnore);

    float_t value;
    readQDataStream(*m_Data, value);
    return value;
  }
  else {
    MOBase::log::warn("Please create an issue on the MO github labeled \"Found unknown Compressed\" with your savefile attached");
    return 0;
  }
}

QStringList GamebryoSaveGame::FileWrapper::readPlugins(int bytesToIgnore)
{
  QStringList plugins;
  if (m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint8_t count;
    read(count);
    uint16_t finalCount = count;
    plugins.reserve(finalCount);
    for (std::size_t i = 0; i < finalCount; ++i) {
      QString name;
      read(name);
      plugins.push_back(name);
    }
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    m_Data->skipRawData(bytesToIgnore);
    uint8_t count;
    readQDataStream(*m_Data, count);
    uint16_t finalCount = count;
    plugins.reserve(finalCount);
    for (std::size_t i = 0; i<finalCount; ++i) {
      QString name;
      readQDataStream(*m_Data, name);
      plugins.push_back(name);
    }
  }
  return plugins;
}

QStringList GamebryoSaveGame::FileWrapper::readLightPlugins(int bytesToIgnore)
{
  QStringList plugins;
  if (m_CompressionType == 0) {
    if (bytesToIgnore>0)//Just to make certain
      skip<char>(bytesToIgnore);
    uint16_t count;
    read(count);
    plugins.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
      QString name;
      read(name);
      plugins.push_back(name);
    }
  } else if (m_CompressionType == 1 || m_CompressionType == 2) {
    m_Data->skipRawData(bytesToIgnore);

    uint16_t count;
    readQDataStream(*m_Data, count);
    plugins.reserve(count);
    for (std::size_t i = 0; i<count; ++i) {
      QString name;
      readQDataStream(*m_Data, name);
      plugins.push_back(name);
    }
  }
  return plugins;
}

void GamebryoSaveGame::FileWrapper::close()
{
  m_File.close();
}