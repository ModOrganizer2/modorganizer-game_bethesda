#include "morrowinddataarchives.h"
#include <utility.h>

MorrowindDataArchives::MorrowindDataArchives(const QDir &myGamesDir) :
  GamebryoDataArchives(myGamesDir)
{
}

QStringList MorrowindDataArchives::vanillaArchives() const
{
  return  { "Morrowind.bsa" };
}


QStringList MorrowindDataArchives::archives(const MOBase::IProfile *profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled() ? QDir(profile->absolutePath()).absoluteFilePath("morrowind.ini") : m_LocalGameDir.absoluteFilePath("morrowind.ini");
  result.append(getArchivesFromKey(iniFile, "SResourceArchiveList"));
  result.append(getArchivesFromKey(iniFile, "SResourceArchiveList2"));

  return result;
}

void MorrowindDataArchives::writeArchiveList(MOBase::IProfile *profile, const QStringList &before)
{
  QString list = before.join(", ");

  QString iniFile = profile->localSettingsEnabled() ? QDir(profile->absolutePath()).absoluteFilePath("morrowind.ini") : m_LocalGameDir.absoluteFilePath("morrowind.ini");
  if (list.length() > 255) {
    int splitIdx = list.lastIndexOf(",", 256);
    setArchivesToKey(iniFile, "SResourceArchiveList", list.mid(0, splitIdx));
    setArchivesToKey(iniFile, "SResourceArchiveList2", list.mid(splitIdx + 2));
  } else {
    setArchivesToKey(iniFile, "SResourceArchiveList", list);
  }
}
