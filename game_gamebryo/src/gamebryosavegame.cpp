#include "gamebryosavegame.h"

#include <QFile>
#include <QFileInfo>

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

void GamebryoSaveGame::readHeader(QFile &file, const QString &expected)
{
  file.setFileName(m_FileName);
  if (!file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error(QObject::tr("failed to open %1").arg(m_FileName).toUtf8().constData());
  }

  std::vector<char> fileID(expected.length() + 1);
  file.read(fileID.data(), expected.length());
  fileID[expected.length()] = '\0';

  QString id(fileID.data());
  if (expected != id) {
    throw std::runtime_error(
          QObject::tr("wrong file format - expected %1 got %2").arg(expected).arg(id).toUtf8().constData());
  }
}

GamebryoSaveGame::FileWrapper::FileWrapper(GamebryoSaveGame *game,
                                           QString const &expected) :
  m_Game(game),
  m_File(game->m_FileName),
  m_HasFieldMarkers(false)
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
