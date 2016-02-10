#include "gamebryosavegame.h"

#include "iplugingame.h"
#include "scriptextender.h"

#include <QDate>
#include <QFile>
#include <QFileInfo>
#include <QScopedArrayPointer>
#include <QTime>

#include <Windows.h>

#include <stdexcept>
#include <vector>

GamebryoSaveGame::GamebryoSaveGame(QString const &file, MOBase::IPluginGame const *game) :
  m_FileName(file),
  m_CreationTime(QFileInfo(file).lastModified()),
  m_Game(game)
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
  }
  return res;
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
  m_BZString(false)
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

void GamebryoSaveGame::FileWrapper::setBZString(bool state)
{
  m_BZString = state;
}

template <> void GamebryoSaveGame::FileWrapper::read(QString &value)
{
  unsigned short length;
  if (m_BZString) {
    unsigned char len;
    read(len);
    length = len;
  } else {
    read(length);
  }
  std::vector<char> buffer(length);

  read(buffer.data(), length);

  if (m_BZString) {
    length -= 1;
  }

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
    // NB Note that scaling rather messes up oblivion so we can't use this as
    // the default
    m_Game->m_Screenshot = image.scaledToWidth(scale);
  } else {
    // why do I have to copy here? without the copy, the buffer seems to get
    // deleted after the temporary vanishes, but shouldn't Qts implicit sharing
    // handle that?
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
