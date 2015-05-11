#include "obliviondataarchives.h"
#include <utility.h>
#include <QDir>


QStringList OblivionDataArchives::vanillaArchives() const
{
  return  { "Oblivion - Misc.bsa"
          , "Oblivion - Textures - Compressed.bsa"
          , "Oblivion - Meshes.bsa"
          , "Oblivion - Sounds.bsa"
          , "Oblivion - Voices1.bsa"
          , "Oblivion - Voices2.bsa"
  };
}


QStringList OblivionDataArchives::archives(const MOBase::IProfile *profile) const
{
  QStringList result;

  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath("oblivion.ini");
  result.append(getArchivesFromKey(iniFile, "SResourceArchiveList"));
  result.append(getArchivesFromKey(iniFile, "SResourceArchiveList2"));

  return result;
}

void OblivionDataArchives::writeArchiveList(MOBase::IProfile *profile, const QStringList &before)
{
  QString list = before.join(", ");

  QString iniFile = QDir(profile->absolutePath()).absoluteFilePath("oblivion.ini");
  if (list.length() > 255) {
    int splitIdx = list.lastIndexOf(",", 256);
    setArchivesToKey(iniFile, "SResourceArchiveList", list.mid(0, splitIdx));
    setArchivesToKey(iniFile, "SResourceArchiveList2", list.mid(splitIdx + 2));
  } else {
    setArchivesToKey(iniFile, "SResourceArchiveList", list);
  }
}
