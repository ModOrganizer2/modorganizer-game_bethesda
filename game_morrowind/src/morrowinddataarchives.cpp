#include "morrowinddataarchives.h"
#include "registry.h"
#include <utility.h>

MorrowindDataArchives::MorrowindDataArchives(const MOBase::IPluginGame* game)
    : GamebryoDataArchives(
          QDir())  // m_LocalGameDir is not used as it's determined too soon
      ,
      m_GamePlugin(game)
{}

QStringList MorrowindDataArchives::vanillaArchives() const
{
  return {"Morrowind.bsa"};
}

QStringList MorrowindDataArchives::getArchives(const QString& iniFile) const
{
  wchar_t buffer[256];
  QStringList result;
  std::wstring iniFileW = QDir::toNativeSeparators(iniFile).toStdWString();

  errno = 0;

  QString key = "Archive ";
  int i       = 0;
  while (::GetPrivateProfileStringW(L"Archives",
                                    (key + QString::number(i)).toStdWString().c_str(),
                                    L"", buffer, 256, iniFileW.c_str()) != 0) {
    result.append(QString::fromStdWString(buffer).trimmed());
    i++;
  }

  return result;
}

void MorrowindDataArchives::setArchives(const QString& iniFile, const QStringList& list)
{
  ::WritePrivateProfileSectionW(L"Archives", NULL, iniFile.toStdWString().c_str());

  QString key      = "Archive ";
  int writtenCount = 0;
  foreach (const QString& value, list) {
    if (!MOBase::WriteRegistryValue(
            L"Archives", (key + QString::number(writtenCount)).toStdWString().c_str(),
            value.toStdWString().c_str(), iniFile.toStdWString().c_str())) {
      qWarning("failed to set archives in \"%s\"", qUtf8Printable(iniFile));
    }
    ++writtenCount;
  }
}

QStringList MorrowindDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath("morrowind.ini")
          : m_GamePlugin->gameDirectory().absoluteFilePath("morrowind.ini");
  result.append(getArchives(iniFile));

  return result;
}

void MorrowindDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                             const QStringList& before)
{
  QString iniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath("morrowind.ini")
          : m_GamePlugin->gameDirectory().absoluteFilePath("morrowind.ini");
  setArchives(iniFile, before);
}
