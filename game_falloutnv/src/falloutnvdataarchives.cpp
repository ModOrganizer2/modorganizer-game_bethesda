#include "falloutnvdataarchives.h"
#include <utility.h>
#include <QDir>


QStringList FalloutNVDataArchives::vanillaArchives() const
{
  return { "Fallout - Textures.bsa"
         , "Fallout - Textures2.bsa"
         , "Fallout - Meshes.bsa"
         , "Fallout - Voices1.bsa"
         , "Fallout - Sound.bsa"
         , "Fallout - Misc.bsa" };
}


QStringList FalloutNVDataArchives::archives(const MOBase::IProfile *profile) const
{
  QStringList result;

  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath("falloutnv.ini");
  result.append(getArchivesFromKey(iniFile, "SArchiveList"));

  return result;
}

void FalloutNVDataArchives::writeArchiveList(MOBase::IProfile *profile, QStringList before)
{
  QString list = before.join(',');

  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath("falloutnv.ini");
  setArchivesToKey(iniFile, "SArchiveList", list);
}
