#include "fallout3dataarchives.h"

#include "iprofile.h"
#include <utility.h>

#include <QDir>


QStringList Fallout3DataArchives::vanillaArchives() const
{
  return { "Fallout - Textures.bsa"
         , "Fallout - Meshes.bsa"
         , "Fallout - Voices.bsa"
         , "Fallout - Sound.bsa"
         , "Fallout - MenuVoices.bsa"
         , "Fallout - Misc.bsa" };
}


QStringList Fallout3DataArchives::archives(const MOBase::IProfile *profile) const
{
  QStringList result;

  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath("fallout3.ini");
  result.append(getArchivesFromKey(iniFile, "SArchiveList"));

  return result;
}

void Fallout3DataArchives::writeArchiveList(MOBase::IProfile *profile, const QStringList &before)
{
  QString list = before.join(", ");

  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath("fallout3.ini");
  setArchivesToKey(iniFile, "SArchiveList", list);
}
