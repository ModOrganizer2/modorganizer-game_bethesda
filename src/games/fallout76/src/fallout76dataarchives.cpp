#include "fallout76dataarchives.h"

#include "iprofile.h"
#include <utility.h>

Fallout76DataArchives::Fallout76DataArchives(const QDir &myGamesDir) :
  GamebryoDataArchives(myGamesDir)
{}

QStringList Fallout76DataArchives::vanillaArchives() const
{
  return { "SeventySix - Animations.ba2"
         , "SeventySix - ATX_Main.ba2"
         , "SeventySix - ATX_Textures.ba2"
         , "SeventySix - EnlightenExteriors01.ba2"
         , "SeventySix - EnlightenExteriors02.ba2"
         , "SeventySix - EnlightenInteriors.ba2"
         , "SeventySix - GeneratedMeshes.ba2"
         , "SeventySix - GeneratedTextures.ba2"
         , "SeventySix - Interface.ba2"
         , "SeventySix - Localization.ba2"
         , "SeventySix - Materials.ba2"
         , "SeventySix - Meshes01.ba2"
         , "SeventySix - Meshes02.ba2"
         , "SeventySix - MeshesExtra.ba2"
         , "SeventySix - MiscClient.ba2"
         , "SeventySix - Shaders.ba2"
         , "SeventySix - Sounds01.ba2"
         , "SeventySix - Sounds02.ba2"
         , "SeventySix - Startup.ba2"
         , "SeventySix - Textures01.ba2"
         , "SeventySix - Textures02.ba2"
         , "SeventySix - Textures03.ba2"
         , "SeventySix - Textures04.ba2"
         , "SeventySix - Textures05.ba2"
         , "SeventySix - Textures06.ba2"
         , "SeventySix - Voices.ba2" };
}

QStringList Fallout76DataArchives::sResourceIndexFileList() const
{
  return { "SeventySix - Textures01.ba2"
         , "SeventySix - Textures02.ba2"
         , "SeventySix - Textures03.ba2"
         , "SeventySix - Textures04.ba2"
         , "SeventySix - Textures05.ba2"
         , "SeventySix - Textures06.ba2" };
}

QStringList Fallout76DataArchives::sResourceStartUpArchiveList() const
{
  return { "SeventySix - Interface.ba2"
         , "SeventySix - Localization.ba2"
         , "SeventySix - Shaders.ba2"
         , "SeventySix - Startup.ba2" };
}

QStringList Fallout76DataArchives::SResourceArchiveList() const {
  return { "SeventySix - GeneratedMeshes.ba2"
         , "SeventySix - Materials.ba2"
         , "SeventySix - Meshes01.ba2"
         , "SeventySix - Meshes02.ba2"
         , "SeventySix - MeshesExtra.ba2"
         , "SeventySix - MiscClient.ba2"
         , "SeventySix - Sounds01.ba2"
         , "SeventySix - Sounds02.ba2"
         , "SeventySix - Startup.ba2"
         , "SeventySix - Voices.ba2" };
}

QStringList Fallout76DataArchives::SResourceArchiveList2() const {
  return { "SeventySix - Animations.ba2"
         , "SeventySix - EnlightenInteriors.ba2"
         , "SeventySix - GeneratedTextures.ba2"
         , "SeventySix - EnlightenExteriors01.ba2"
         , "SeventySix - EnlightenExteriors02.ba2" };
}

QStringList Fallout76DataArchives::SResourceArchiveMemoryCacheList() const {
  return { "SeventySix - Interface.ba2"
         , "SeventySix - Materials.ba2"
         , "SeventySix - MiscClient.ba2"
         , "SeventySix - Shaders.ba2" };
}

QStringList Fallout76DataArchives::sResourceArchive2List() const {
  return { "SeventySix - ATX_Main.ba2"
         , "SeventySix - ATX_Textures.ba2" };
}

QStringList Fallout76DataArchives::archives(const MOBase::IProfile *profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled() ? QDir(profile->absolutePath()).absoluteFilePath("Fallout76.ini") : m_LocalGameDir.absoluteFilePath("Fallout76.ini");
  result.append(getArchivesFromKey(iniFile, "SResourceArchiveList"));
  result.append(getArchivesFromKey(iniFile, "SResourceArchiveList2"));

  return result;
}

void Fallout76DataArchives::writeArchiveList(MOBase::IProfile *profile, const QStringList &before)
{
  QString list = before.join(", ");

  QString iniFile = profile->localSettingsEnabled() ? QDir(profile->absolutePath()).absoluteFilePath("Fallout76.ini") : m_LocalGameDir.absoluteFilePath("Fallout76.ini");
//  if (list.length() > 255) {
//    int splitIdx = list.lastIndexOf(",", 256);
//    setArchivesToKey(iniFile, "SResourceArchiveList", list.mid(0, splitIdx));
//    setArchivesToKey(iniFile, "SResourceArchiveList2", list.mid(splitIdx + 2));
//  } else {
    setArchivesToKey(iniFile, "sResourceArchive2List", list);
//  }
}
