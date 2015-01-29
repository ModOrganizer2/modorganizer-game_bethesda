#include "gamebryodataarchives.h"
#include <QDir>
#include <Windows.h>
#include <utility.h>


QStringList GamebryoDataArchives::getArchivesFromKey(const QString &iniFile, const QString &key) const
{
  wchar_t buffer[256];
  QStringList result;
  std::wstring iniFileW = QDir::toNativeSeparators(iniFile).toStdWString();

  // epic ms fail: GetPrivateProfileString uses errno (for whatever reason) to signal a fail since the return value
  // has a different meaning (number of bytes copied). HOWEVER, it will not set errno to 0 if NO error occured
  errno = 0;

  if (::GetPrivateProfileStringW(L"Archive", key.toStdWString().c_str(),
                                 L"", buffer, 256, iniFileW.c_str()) != 0) {
    result.append(QString::fromStdWString(buffer).split(','));
  }

  for (int i = 0; i < result.count(); ++i) {
    result[i] = result[i].trimmed();
  }
  return result;
}

void GamebryoDataArchives::setArchivesToKey(const QString &iniFile, const QString &key, const QString &value)
{
  if (!::WritePrivateProfileStringW(L"Archive", key.toStdWString().c_str(), value.toStdWString().c_str(), iniFile.toStdWString().c_str())) {
    throw MOBase::MyException(QObject::tr("failed to set archive key (errorcode %1)").arg(errno));
  }
}

void GamebryoDataArchives::addArchive(MOBase::IProfile *profile, int index, const QString &archiveName)
{
  QStringList current = archives(profile);
  if (current.contains(archiveName, Qt::CaseInsensitive)) {
    return;
  }

  current.insert(index != INT_MAX ? index : current.size(), archiveName);

  writeArchiveList(profile, current);
}

void GamebryoDataArchives::removeArchive(MOBase::IProfile *profile, const QString &archiveName)
{
  QStringList current = archives(profile);
  if (!current.contains(archiveName, Qt::CaseInsensitive)) {
    return;
  }

  current.removeAll(archiveName);

  writeArchiveList(profile, current);
}
