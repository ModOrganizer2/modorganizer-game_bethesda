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


GamebryoBSAInvalidation::GamebryoBSAInvalidation(DataArchives *dataArchives
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
  prepareProfile(profile);
}

void GamebryoBSAInvalidation::activate(MOBase::IProfile *profile)
{
  prepareProfile(profile);
}

bool GamebryoBSAInvalidation::prepareProfile(MOBase::IProfile *profile)
{
  bool dirty = false;
  QString basePath
          = profile->localSettingsEnabled()
            ? profile->absolutePath()
            : m_Game->documentsDirectory().absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;
  WCHAR setting[MAX_PATH];

  // write bInvalidateOlderFiles = 1, if needed
  if (!::GetPrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"0", setting, MAX_PATH, iniFilePath.toStdWString().c_str())
    || wcstol(setting, nullptr, 10) != 1) {
    dirty = true;
    if (!::WritePrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"1", iniFilePath.toStdWString().c_str())) {
      throw MOBase::MyException(QObject::tr("failed to activate BSA invalidation in \"%1\" (errorcode %2)").arg(m_IniFileName, ::GetLastError()));
    }
  }

  if (profile->invalidationActive(nullptr)){

    // add the dummy bsa to the archive string, if needed
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
      dirty = true;
    }

    // create the dummy bsa if necessary
    QString bsaFile = m_Game->dataDirectory().absoluteFilePath(invalidationBSAName());
    if (!QFile::exists(bsaFile)) {
      DummyBSA bsa(bsaVersion());
      bsa.write(bsaFile);
      dirty = true;
    }

    // write SInvalidationFile = "", if needed
    if (::GetPrivateProfileStringW(L"Archive", L"SInvalidationFile", L"ArchiveInvalidation.txt", setting, MAX_PATH, iniFilePath.toStdWString().c_str())
      || wcscmp(setting, L"") != 0) {
      dirty = true;
      if (!::WritePrivateProfileStringW(L"Archive", L"SInvalidationFile", L"", iniFilePath.toStdWString().c_str())) {
        throw MOBase::MyException(QObject::tr("failed to activate BSA invalidation in \"%1\" (errorcode %2)").arg(m_IniFileName, ::GetLastError()));
      }
    }
  } else {

    // remove the dummy bsa from the archive string, if needed
    QStringList archivesBefore = m_DataArchives->archives(profile);
    for (const QString &archive : archivesBefore) {
      if (isInvalidationBSA(archive)) {
        m_DataArchives->removeArchive(profile, archive);
        dirty = true;
      }
    }

    // delete the dummy bsa, if needed
    QString bsaFile = m_Game->dataDirectory().absoluteFilePath(invalidationBSAName());
    if (QFile::exists(bsaFile)) {
      MOBase::shellDeleteQuiet(bsaFile);
      dirty = true;
    }

    // write SInvalidationFile = "ArchiveInvalidation.txt", if needed
    if (!::GetPrivateProfileStringW(L"Archive", L"SInvalidationFile", L"", setting, MAX_PATH, iniFilePath.toStdWString().c_str())
      || wcscmp(setting, L"ArchiveInvalidation.txt") != 0) {
      dirty = true;
      if (!::WritePrivateProfileStringW(L"Archive", L"SInvalidationFile", L"ArchiveInvalidation.txt", iniFilePath.toStdWString().c_str())) {
        throw MOBase::MyException(QObject::tr("failed to activate BSA invalidation in \"%1\" (errorcode %2)").arg(m_IniFileName, ::GetLastError()));
      }
    }
  }

  return dirty;
}
