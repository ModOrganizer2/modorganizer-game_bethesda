#include "gameoblivion.h"

#include "oblivionbsainvalidation.h"
#include "obliviondataarchives.h"
#include "oblivionscriptextender.h"
#include "oblivionmoddatachecker.h"
#include "oblivionmoddatacontent.h"
#include "oblivionsavegame.h"

#include "pluginsetting.h"
#include "executableinfo.h"
#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include <gamebryounmanagedmods.h>
#include <gamebryosavegameinfo.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <memory>

using namespace MOBase;

GameOblivion::GameOblivion()
{
}

bool GameOblivion::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  registerFeature<ScriptExtender>(new OblivionScriptExtender(this));
  registerFeature<DataArchives>(new OblivionDataArchives(myGamesPath()));
  registerFeature<BSAInvalidation>(new OblivionBSAInvalidation(feature<DataArchives>(), this));
  registerFeature<SaveGameInfo>(new GamebryoSaveGameInfo(this));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "oblivion.ini"));
  registerFeature<ModDataChecker>(new OblivionModDataChecker(this));
  registerFeature<ModDataContent>(new OblivionModDataContent(this));
  registerFeature<GamePlugins>(new GamebryoGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new GamebryoUnmangedMods(this));
  return true;
}

QString GameOblivion::gameName() const
{
  return "Oblivion";
}

QList<ExecutableInfo> GameOblivion::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("Oblivion", findInGameFolder(binaryName()))
      << ExecutableInfo("Oblivion Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("Oblivion Mod Manager", findInGameFolder("OblivionModManager.exe"))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", QFileInfo(getLootPath())).withArgument("--game=\"Oblivion\"")
      << ExecutableInfo("Construction Set", findInGameFolder("TESConstructionSet.exe"))
  ;
}

QList<ExecutableForcedLoadSetting> GameOblivion::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>()
      << ExecutableForcedLoadSetting("Oblivion.exe", "obse_1_2_416.dll").withForced().withEnabled()
      << ExecutableForcedLoadSetting("TESConstructionSet.exe", "obse_editor_1_2.dll").withForced().withEnabled()
  ;
}

QString GameOblivion::name() const
{
  return "Oblivion Support Plugin";
}

QString GameOblivion::localizedName() const
{
  return tr("Oblivion Support Plugin");
}

QString GameOblivion::author() const
{
  return "Tannin";
}

QString GameOblivion::description() const
{
  return tr("Adds support for the game Oblivion");
}

MOBase::VersionInfo GameOblivion::version() const
{
  return VersionInfo(1, 6, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameOblivion::settings() const
{
  return {
      PluginSetting("nehrim_downloads", "allow Nehrim downloads", QVariant(false))
  };
}

void GameOblivion::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Oblivion", path, "plugins.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/oblivion.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "oblivion_default.ini", "oblivion.ini");
    } else {
      copyToProfile(myGamesPath(), path, "oblivion.ini");
    }

    copyToProfile(myGamesPath(), path, "oblivionprefs.ini");
  }
}

QString GameOblivion::savegameExtension() const
{
  return "ess";
}

QString GameOblivion::savegameSEExtension() const
{
  return "obse";
}

std::shared_ptr<const GamebryoSaveGame> GameOblivion::makeSaveGame(QString filePath) const
{
  return std::make_shared<const OblivionSaveGame>(filePath, this);
}

QString GameOblivion::steamAPPId() const
{
  return "22330";
}

QStringList GameOblivion::primaryPlugins() const
{
  return { "oblivion.esm", "update.esm" };
}

QString GameOblivion::gameShortName() const
{
  return "Oblivion";
}

QStringList GameOblivion::validShortNames() const
{
  QStringList shortNames;
  if (m_Organizer->pluginSetting(name(), "nehrim_downloads").toBool()) {
    shortNames.append( "Nehrim" );
  }
  return shortNames;
}

QString GameOblivion::gameNexusName() const
{
  return "Oblivion";
}


QStringList GameOblivion::iniFiles() const
{
  return { "oblivion.ini", "oblivionprefs.ini" };
}

QStringList GameOblivion::DLCPlugins() const
{
  return { "DLCBattlehornCastle.esp", "DLCShiveringIsles.esp", "Knights.esp", "DLCFrostcrag.esp",
           "DLCSpellTomes.esp", "DLCMehrunesRazor.esp", "DLCOrrery.esp",
           "DLCThievesDen.esp", "DLCVileLair.esp", "DLCHorseArmor.esp"  };
}


int GameOblivion::nexusModOrganizerID() const
{
  return 38277;
}

int GameOblivion::nexusGameID() const
{
  return 101;
}
