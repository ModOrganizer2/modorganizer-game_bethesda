#include "gamenehrim.h"

#include "nehrimbsainvalidation.h"
#include "nehrimdataarchives.h"
#include "nehrimscriptextender.h"
#include "nehrimmoddatachecker.h"
#include "nehrimmoddatacontent.h"
#include "nehrimsavegame.h"

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

GameNehrim::GameNehrim()
{
}

bool GameNehrim::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  registerFeature<ScriptExtender>(new NehrimScriptExtender(this));
  registerFeature<DataArchives>(new NehrimDataArchives(myGamesPath()));
  registerFeature<BSAInvalidation>(new NehrimBSAInvalidation(feature<DataArchives>(), this));
  registerFeature<SaveGameInfo>(new GamebryoSaveGameInfo(this));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "oblivion.ini"));
  registerFeature<ModDataChecker>(new NehrimModDataChecker(this));
  registerFeature<ModDataContent>(new NehrimModDataContent(this));
  registerFeature<GamePlugins>(new GamebryoGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new GamebryoUnmangedMods(this));
  return true;
}

QString GameNehrim::gameName() const
{
  return "Nehrim";
}

QList<ExecutableInfo> GameNehrim::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("Nehrim", findInGameFolder(binaryName()))
      << ExecutableInfo("Nehrim Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("Oblivion Mod Manager", findInGameFolder("OblivionModManager.exe"))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Oblivion\"")
      << ExecutableInfo("Construction Set", findInGameFolder("TESConstructionSet.exe"))
  ;
}

QList<ExecutableForcedLoadSetting> GameNehrim::executableForcedLoads() const
{
  //TODO Search game directory for OBSE DLLs
  return QList<ExecutableForcedLoadSetting>()
      << ExecutableForcedLoadSetting("Oblvion.exe", "obse_1_2_416.dll").withForced()
      << ExecutableForcedLoadSetting("TESConstructionSet.exe", "obse_editor_1_2.dll").withForced()
  ;
}

QString GameNehrim::name() const
{
  return "Nehrim Support Plugin";
}

QString GameNehrim::localizedName() const
{
  return tr("Nehrim Support Plugin");
}

QString GameNehrim::author() const
{
  return "Tannin";
}

QString GameNehrim::description() const
{
  return tr("Adds support for the game Nehrim");
}

MOBase::VersionInfo GameNehrim::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameNehrim::settings() const
{
  return QList<PluginSetting>();
}

void GameNehrim::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Oblvion", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Oblvion", path, "loadorder.txt");
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

QString GameNehrim::savegameExtension() const
{
  return "ess";
}

QString GameNehrim::savegameSEExtension() const
{
  return "obse";
}

std::shared_ptr<const GamebryoSaveGame> GameNehrim::makeSaveGame(QString filePath) const
{
  return std::make_shared<const NehrimSaveGame>(filePath, this);
}

QString GameNehrim::steamAPPId() const
{
  return "22330";
}

QStringList GameNehrim::primaryPlugins() const
{
  return { "Nehrim.esm", "Translation.esp" };
}

QString GameNehrim::gameShortName() const
{
  return "Nehrim";
}

QString GameNehrim::gameNexusName() const
{
  return "Nehrim";
}

QStringList GameNehrim::iniFiles() const
{
  return { "oblivion.ini", "oblivionprefs.ini" };
}

QStringList GameNehrim::DLCPlugins() const
{
  return {};
}


int GameNehrim::nexusModOrganizerID() const
{
  return -1;
}

int GameNehrim::nexusGameID() const
{
  return 3312;
}

QStringList GameNehrim::primarySources() const
{
    return {"Oblivion"};
}

QStringList GameNehrim::validShortNames() const
{
    return {"Oblivion"};
}

QString GameNehrim::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\Oblivion";
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(), L"Installed Path");
}

QString GameNehrim::binaryName() const
{
    return "Oblivion.exe";
}

QIcon GameNehrim::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath("NehrimLauncher.exe"));
}