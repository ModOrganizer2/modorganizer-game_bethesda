#include "gamebryosavegame.h"

#include <QFile>
#include <QFileInfo>

#include "Windows.h"

#include <stdexcept>
#include <vector>

GamebryoSaveGame::GamebryoSaveGame(QString const &file) :
  m_FileName(file),
  m_CreationTime(QFileInfo(file).lastModified())
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

QString GamebryoSaveGame::getIdentifier() const
{
  return m_PCName;
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
  m_Length(2)
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

void GamebryoSaveGame::FileWrapper::setStringLength(size_t len)
{
  m_Length = len;
}

template <> __declspec(dllexport) void GamebryoSaveGame::FileWrapper::read(QString &value)
{
  unsigned short length;
  if (m_Length == 1) {
    unsigned char len;
    read(len);
    length = len;
  } else {
    read(length);
  }
  std::vector<char> buffer(length);

  read(buffer.data(), length);
  if (m_HasFieldMarkers) {
    skip<char>();
  }

  value = QString::fromLatin1(buffer.data(), length);
}

void GamebryoSaveGame::FileWrapper::read(void *buff, std::size_t length)
{
  int read = m_File.read(static_cast<char *>(buff), length);
  if (read != length) {
    throw std::runtime_error("unexpected end of file");
  }
}

void GamebryoSaveGame::FileWrapper::readImage(int scale)
{
  unsigned long width;
  read(width);
  unsigned long height;
  read(height);
  readImage(width, height, scale);
}

void GamebryoSaveGame::FileWrapper::readImage(unsigned long width, unsigned long height, int scale)
{
  QScopedArrayPointer<unsigned char> buffer(new unsigned char[width * height * 3]);
  read(buffer.data(), width * height * 3);
  QImage image(buffer.data(), width, height, QImage::Format_RGB888);
  if (scale) {
    m_Game->m_Screenshot = image.scaledToWidth(scale);
  } else {
    // why do I have to copy here? without the copy, the buffer seems to get deleted after the
    // temporary vanishes, but Qts implicit sharing should handle that?
    m_Game->m_Screenshot = image.copy();
  }
}

void GamebryoSaveGame::FileWrapper::readPlugins()
{
  unsigned char count;
  read(count);
  for (std::size_t i = 0; i < count; ++i) {
    QString name;
    read(name);
    m_Game->m_Plugins.push_back(name);
  }
}
