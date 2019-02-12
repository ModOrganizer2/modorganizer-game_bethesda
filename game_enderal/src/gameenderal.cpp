#include "gameenderal.h"

#include "enderalbsainvalidation.h"
#include "enderalscriptextender.h"
#include "enderaldataarchives.h"
#include "enderalsavegameinfo.h"
#include "enderalgameplugins.h"

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

GameEnderal::GameEnderal()
{
}

bool GameEnderal::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  registerFeature<ScriptExtender>(new EnderalScriptExtender(this));
  registerFeature<DataArchives>(new EnderalDataArchives(myGamesPath()));
  registerFeature<BSAInvalidation>(new EnderalBSAInvalidation(feature<DataArchives>(), this));
  registerFeature<SaveGameInfo>(new EnderalSaveGameInfo(this));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "enderal.ini"));
  registerFeature<GamePlugins>(new EnderalGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new GamebryoUnmangedMods(this));
  return true;
}

QString GameEnderal::gameName() const
{
  return "Enderal";
}

QList<ExecutableInfo> GameEnderal::executables() const
{
  return QList<ExecutableInfo>()
      //<< ExecutableInfo("SKSE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
      //<< ExecutableInfo("SBW", findInGameFolder("SBW.exe"))
      << ExecutableInfo("Enderal (SKSE)", findInGameFolder(binaryName()))
      << ExecutableInfo("Enderal Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      //<< ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Skyrim\"")
      << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe")).withSteamAppId("202480")
  ;
}

QList<ExecutableForcedLoadSetting> GameEnderal::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameEnderal::name() const
{
  return "Enderal Support Plugin";
}

QString GameEnderal::author() const
{
  return "AL12";
}

QString GameEnderal::description() const
{
  return tr("Adds support for the game Enderal");
}

MOBase::VersionInfo GameEnderal::version() const
{
  return VersionInfo(0, 0, 1, VersionInfo::RELEASE_PREALPHA);
}

bool GameEnderal::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameEnderal::settings() const
{
  return QList<PluginSetting>();
}

void GameEnderal::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/enderal", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/enderal", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/enderal.ini").exists()) {
			
	    //there is no defalt ini, actually they are going to put them in for us!
      copyToProfile(gameDirectory().absolutePath(), path, "enderal_default.ini", "enderal.ini");
      copyToProfile(gameDirectory().absolutePath(), path, "enderalprefs_default.ini", "enderalprefs.ini");
    } else {
      copyToProfile(myGamesPath(), path, "enderal.ini");
      copyToProfile(myGamesPath(), path, "enderalprefs.ini");
    }    
  }
}

QString GameEnderal::savegameExtension() const
{
  return "ess";
}

QString GameEnderal::savegameSEExtension() const
{
  return "skse";
}

QString GameEnderal::steamAPPId() const
{
  return "933480";
}

QStringList GameEnderal::primaryPlugins() const
{
  return { "Skyrim.esm", "update.esm", "Enderal - Forgotten Stories.esm" };
}

QString GameEnderal::binaryName() const
{
  return "skse_loader.exe";
}

QString GameEnderal::getLauncherName() const
{
  return "Enderal Launcher.exe";
}

QString GameEnderal::gameShortName() const
{
  return "Enderal";
}

QString GameEnderal::gameNexusName() const
{
  return "Skyrim";
}

QStringList GameEnderal::primarySources() const
{
  return {"Skyrim"};
}

QStringList GameEnderal::validShortNames() const
{
  return {"Skyrim"};
}

QStringList GameEnderal::iniFiles() const
{
  return { "enderal.ini", "enderalprefs.ini" };
}

QStringList GameEnderal::DLCPlugins() const
{
  return {};
}

namespace {
//Note: This is ripped off from shared/util. And in an upcoming move, the fomod
//installer requires something similar. I suspect I should abstract this out
//into gamebryo (or lower level)
//Unused for Enderal

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

IPluginGame::LoadOrderMechanism GameEnderal::loadOrderMechanism() const
{
  return LoadOrderMechanism::PluginsTxt;
}


int GameEnderal::nexusModOrganizerID() const
{
  return 1334;
}

int GameEnderal::nexusGameID() const
{
  return 110;
}
