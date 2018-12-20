#include "gamefallout3.h"

#include "fallout3bsainvalidation.h"
#include "fallout3scriptextender.h"
#include "fallout3dataarchives.h"
#include "fallout3savegameinfo.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "versioninfo.h"
#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

using namespace MOBase;

GameFallout3::GameFallout3()
{
}

bool GameFallout3::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  registerFeature<ScriptExtender>(new Fallout3ScriptExtender(this));
  registerFeature<DataArchives>(new Fallout3DataArchives(myGamesPath()));
  registerFeature<BSAInvalidation>(new Fallout3BSAInvalidation(feature<DataArchives>(), this));
  registerFeature<SaveGameInfo>(new Fallout3SaveGameInfo(this));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "fallout.ini"));
  registerFeature<GamePlugins>(new GamebryoGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new GamebryoUnmangedMods(this));
  return true;
}

QString GameFallout3::gameName() const
{
  return "Fallout 3";
}

QList<ExecutableInfo> GameFallout3::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("FOSE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
      << ExecutableInfo("Fallout 3", findInGameFolder(binaryName()))
      << ExecutableInfo("Fallout Mod Manager", findInGameFolder("fomm/fomm.exe"))
      << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
      << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Fallout3\"")
         ;
}

QString GameFallout3::name() const
{
  return "Fallout 3 Support Plugin";
}

QString GameFallout3::author() const
{
  return "Tannin";
}

QString GameFallout3::description() const
{
  return tr("Adds support for the game Fallout 3s");
}

MOBase::VersionInfo GameFallout3::version() const
{
  return VersionInfo(1, 3, 0, VersionInfo::RELEASE_FINAL);
}

bool GameFallout3::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameFallout3::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout3::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Fallout3", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Fallout3", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/fallout.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "fallout_default.ini", "fallout.ini");
    } else {
      copyToProfile(myGamesPath(), path, "fallout.ini");
    }

    copyToProfile(myGamesPath(), path, "falloutprefs.ini");
	  copyToProfile(myGamesPath(), path, "custom.ini");
    copyToProfile(myGamesPath(), path, "GECKCustom.ini");
    copyToProfile(myGamesPath(), path, "GECKPrefs.ini");
  }
}

QString GameFallout3::savegameExtension() const
{
  return "fos";
}

QString GameFallout3::savegameSEExtension() const
{
  return "";
}

QString GameFallout3::steamAPPId() const
{
  if (selectedVariant() == "Game Of The Year") {
    return "22370";
  } else {
    return "22300";
  }
}

QStringList GameFallout3::primaryPlugins() const
{
  return { "fallout3.esm" };
}


QStringList GameFallout3::gameVariants() const
{
  return { "Regular", "Game Of The Year" };
}

QString GameFallout3::gameShortName() const
{
  return "Fallout3";
}

QString GameFallout3::gameNexusName() const
{
  return "Fallout3";
}


QStringList GameFallout3::iniFiles() const
{
  return { "fallout.ini", "falloutprefs.ini", "custom.ini", "GECKCustom.ini", "GECKPrefs.ini" };
}

QStringList GameFallout3::DLCPlugins() const
{
  return { "ThePitt.esm", "Anchorage.esm", "BrokenSteel.esm", "PointLookout.esm", "Zeta.esm" };
}

int GameFallout3::nexusModOrganizerID() const
{
  return 16348;
}

int GameFallout3::nexusGameID() const
{
  return 120;
}

QString GameFallout3::getLauncherName() const
{
  return "FalloutLauncher.exe";
}
