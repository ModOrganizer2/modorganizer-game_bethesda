#include "gamebryobsainvalidation.h"

#include "dummybsa.h"
#include "iplugingame.h"
#include "iprofile.h"
#include <utility.h>
#include <imoinfo.h>
#include <utility.h>

#include <QStringList>
#include <QDir>

#include <Windows.h>


GamebryoBSAInvalidation::GamebryoBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives
                                                 , const QString &iniFilename
                                                 , MOBase::IPluginGame const *game)
  : m_DataArchives(dataArchives)
  , m_IniFileName(iniFilename)
  , m_Game(game)
{
}

bool GamebryoBSAInvalidation::isInvalidationBSA(const QString &bsaName)
{
  static QStringList invalidation { invalidationBSAName() };

  for (const QString &file : invalidation) {
    if (file.compare(bsaName, Qt::CaseInsensitive) == 0) {
      return true;
    }
  }
  return false;
}

void GamebryoBSAInvalidation::deactivate(MOBase::IProfile *profile)
{
  QStringList archivesBefore = m_DataArchives->archives(profile);
  for (const QString &archive : archivesBefore) {
    if (isInvalidationBSA(archive)) {
      m_DataArchives->removeArchive(profile, archive);
    }
  }

  QString bsaFile = m_Game->dataDirectory().absoluteFilePath(invalidationBSAName());
  if (QFile::exists(bsaFile)) {
    MOBase::shellDeleteQuiet(bsaFile);
  }

  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath(m_IniFileName);

  ::SetFileAttributesW(iniFile.toStdWString().c_str(), FILE_ATTRIBUTE_NORMAL);

  if (!::WritePrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"0", iniFile.toStdWString().c_str()) ||
      !::WritePrivateProfileStringW(L"Archive", L"SInvalidationFile", L"ArchiveInvalidation.txt", iniFile.toStdWString().c_str())) {
    throw MOBase::MyException(QObject::tr("failed to deactivate BSA invalidation in \"%1\" (errorcode %2)").arg(iniFile, ::GetLastError()));
  }
}

void GamebryoBSAInvalidation::activate(MOBase::IProfile *profile)
{
  // set the invalidation bsa up to be loaded
  QStringList archives = m_DataArchives->archives(profile);
  bool bsaInstalled = false;
  for (const QString &archive : archives) {
    if (isInvalidationBSA(archive)) {
      bsaInstalled = true;
      break;
    }
  }
  if (!bsaInstalled) {
    m_DataArchives->addArchive(profile, 0, invalidationBSAName());
  }

  // create the dummy bsa if necessary
  QString bsaFile = m_Game->dataDirectory().absoluteFilePath(invalidationBSAName());
  if (!QFile::exists(bsaFile)) {
    DummyBSA bsa(bsaVersion());
    bsa.write(bsaFile);
  }

  // set the remaining ini settings required
  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath(m_IniFileName);

  ::SetFileAttributesW(iniFile.toStdWString().c_str(), FILE_ATTRIBUTE_NORMAL);

  if (!::WritePrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"1", iniFile.toStdWString().c_str()) ||
      !::WritePrivateProfileStringW(L"Archive", L"SInvalidationFile", L"", iniFile.toStdWString().c_str())) {
    throw MOBase::MyException(QObject::tr("failed to activate BSA invalidation in \"%1\" (errorcode %2)").arg(iniFile, ::GetLastError()));
  }
}

