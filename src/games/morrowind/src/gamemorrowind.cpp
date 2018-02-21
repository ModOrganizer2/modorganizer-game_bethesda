#include "gamemorrowind.h"

//#include "skyrimbsainvalidation.h"
//#include "skyrimscriptextender.h"
//#include "skyrimdataarchives.h"
//#include "skyrimsavegameinfo.h"

#include "executableinfo.h"
#include "pluginsetting.h"

#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>

#include <QtDebug>

#include <Windows.h>
#include <winver.h>

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace MOBase;

GameMorrowind::GameMorrowind()
{
}

bool GameMorrowind::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  //registerFeature<ScriptExtender>(new SkyrimScriptExtender(this));
  //registerFeature<DataArchives>(new SkyrimDataArchives(myGamesPath()));
  //registerFeature<BSAInvalidation>(new SkyrimBSAInvalidation(feature<DataArchives>(), this));
  //registerFeature<SaveGameInfo>(new SkyrimSaveGameInfo(this));
  //registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "skyrim.ini"));
  //registerFeature<GamePlugins>(new GamebryoGamePlugins(moInfo));
  //registerFeature<UnmanagedMods>(new GamebryoUnmangedMods(this));
  return true;
}

QString GameMorrowind::gameName() const
{
  return "Morrowind";
}

QDir GameMorrowind::dataDirectory() const
{
  return gameDirectory().absoluteFilePath("Data Files");
}

QDir GameMorrowind::documentsDirectory() const
{
  return gameDirectory();
}

QList<ExecutableInfo> GameMorrowind::executables() const
{
  return QList<ExecutableInfo>()
  //    << ExecutableInfo("SKSE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
  //    << ExecutableInfo("SBW", findInGameFolder("SBW.exe"))
      << ExecutableInfo("Morrowind", findInGameFolder(binaryName()))
      << ExecutableInfo("Morrowind Launcher", findInGameFolder(getLauncherName()))
  //    << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
  //    << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Skyrim\"")
  //    << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe")).withSteamAppId("202480")
  ;
}

QString GameMorrowind::name() const
{
  return "Morrowind Support Plugin";
}

QString GameMorrowind::author() const
{
  return "Schilduin";
}

QString GameMorrowind::description() const
{
  return tr("Adds support for the game Morrowind");
}

MOBase::VersionInfo GameMorrowind::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool GameMorrowind::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameMorrowind::settings() const
{
  return QList<PluginSetting>();
}

void GameMorrowind::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Morrowind", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Morrowind", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/Morrowind.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "Morrowind.ini");
    } else {
      copyToProfile(myGamesPath(), path, "Morrowind.ini");
    }
  }
}

QString GameMorrowind::savegameExtension() const
{
  return "ess";
}

QString GameMorrowind::steamAPPId() const
{
  return "22320";
}

QStringList GameMorrowind::primaryPlugins() const
{
  return { "Morrowind.esm" };
}

QString GameMorrowind::gameShortName() const
{
  return "Morrowind";
}

QString GameMorrowind::getLauncherName() const
{
  return "Morrowind Launcher.exe";
}

QString GameMorrowind::gameNexusName() const
{
  return "Morrowind";
}


QStringList GameMorrowind::iniFiles() const
{
  return { "Morrowind.ini" };
}

QStringList GameMorrowind::DLCPlugins() const
{
  return { "Tribunal.esm", "Bloodmoon.esm" };
}

namespace {
//Note: This is ripped off from shared/util. And in an upcoming move, the fomod
//installer requires something similar. I suspect I should abstract this out
//into gamebryo (or lower level)

VS_FIXEDFILEINFO GetFileVersion(const std::wstring &fileName)
{
  DWORD handle = 0UL;
  DWORD size = ::GetFileVersionInfoSizeW(fileName.c_str(), &handle);
  if (size == 0) {
    throw std::runtime_error("failed to determine file version info size");
  }

  std::vector<char> buffer(size);
  handle = 0UL;
  if (!::GetFileVersionInfoW(fileName.c_str(), handle, size, buffer.data())) {
    throw std::runtime_error("failed to determine file version info");
  }

  void *versionInfoPtr = nullptr;
  UINT versionInfoLength = 0;
  if (!::VerQueryValue(buffer.data(), L"\\", &versionInfoPtr, &versionInfoLength)) {
    throw std::runtime_error("failed to determine file version");
  }

  return *static_cast<VS_FIXEDFILEINFO*>(versionInfoPtr);
}

}

IPluginGame::LoadOrderMechanism GameMorrowind::loadOrderMechanism() const
{
  try {
    std::wstring fileName = gameDirectory().absoluteFilePath(binaryName()).toStdWString().c_str();
    VS_FIXEDFILEINFO versionInfo = ::GetFileVersion(fileName);
    if ((versionInfo.dwFileVersionMS > 0x10004) || // version >= 1.5.x?
        ((versionInfo.dwFileVersionMS == 0x10004) && (versionInfo.dwFileVersionLS >= 0x1A0000))) { // version >= ?.4.26
      return LoadOrderMechanism::PluginsTxt;
    }
  } catch (const std::exception &e) {
    qCritical() << "Morrowind.exe is invalid: " << e.what();
  }
  return LoadOrderMechanism::FileTime;
}


int GameMorrowind::nexusModOrganizerID() const
{
  return 1334;
}

int GameMorrowind::nexusGameID() const
{
  return 100;
}
