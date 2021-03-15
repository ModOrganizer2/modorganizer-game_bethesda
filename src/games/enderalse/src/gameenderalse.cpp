#include "gameenderalse.h"

#include "enderalsedataarchives.h"
#include "enderalsescriptextender.h"
#include "enderalseunmanagedmods.h"
#include "enderalsegameplugins.h"
#include "enderalselocalsavegames.h"
#include "enderalsemoddatachecker.h"
#include "enderalsemoddatacontent.h"
#include "enderalsesavegame.h"
#include "steamutility.h"

#include <pluginsetting.h>
#include <executableinfo.h>
#include <gamebryosavegameinfo.h>
#include "versioninfo.h"
#include <ipluginlist.h>
#include <utility.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>
#include "scopeguard.h"

using namespace MOBase;

GameEnderalSE::GameEnderalSE()
{
}

QString GameEnderalSE::identifyGamePath() const
{
  QString path = "Software\\SureAI\\EnderalSE";
  QString result;
  try {
    result = findInRegistry(HKEY_CURRENT_USER, path.toStdWString().c_str(), L"Install_Path");
  }
  catch (MOBase::MyException) {
    result = MOBase::findSteamGame("Enderal Special Edition", "Data\\Enderal - Forgotten Stories.esm");
  }
  return result;
}

bool GameEnderalSE::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature<ScriptExtender>(new EnderalSEScriptExtender(this));
  registerFeature<DataArchives>(new EnderalSEDataArchives(myGamesPath()));
  registerFeature<LocalSavegames>(new EnderalSELocalSavegames(myGamesPath(), "Enderal.ini"));
  registerFeature<ModDataChecker>(new EnderalSEModDataChecker(this));
  registerFeature<ModDataContent>(new EnderalSEModDataContent(this));
  registerFeature<SaveGameInfo>(new GamebryoSaveGameInfo(this));
  registerFeature<GamePlugins>(new EnderalSEGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new EnderalSEUnmangedMods(this));

  return true;
}

QString GameEnderalSE::gameName() const
{
  return "Enderal Special Edition";
}

QIcon GameEnderalSE::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(getLauncherName()));
}

QList<ExecutableInfo> GameEnderalSE::executables() const
{
  return {
    ExecutableInfo("Enderal Special Edition (SKSE)", findInGameFolder(feature<ScriptExtender>()->loaderName())),
    ExecutableInfo("Enderal Special Edition Launcher", findInGameFolder(getLauncherName())),
    // ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Skyrim Special Edition\""),
    ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
  };
}

QList<ExecutableForcedLoadSetting> GameEnderalSE::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameEnderalSE::binaryName() const
{
  return "skse64_loader.exe";
}

QString GameEnderalSE::getLauncherName() const
{
  return "Enderal Launcher.exe";
}

bool GameEnderalSE::looksValid(const QDir& folder) const
{
  // we need to check both launcher and binary because the binary also exists for
  // Skyrim SE and the launcher for Enderal LE
  return folder.exists(getLauncherName()) && folder.exists(binaryName());
}

QFileInfo GameEnderalSE::findInGameFolder(const QString &relativePath) const
{
  return QFileInfo(m_GamePath + "/" + relativePath);
}

QString GameEnderalSE::name() const
{
  return "Enderal Special Edition Support Plugin";
}

QString GameEnderalSE::localizedName() const
{
  return tr("Enderal Special Edition Support Plugin");
}

QString GameEnderalSE::author() const
{
  return "Holt59, Archost & ZachHaber";
}

QString GameEnderalSE::description() const
{
  return tr("Adds support for the game Enderal Special Edition.");
}

MOBase::VersionInfo GameEnderalSE::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_BETA);
}

QList<PluginSetting> GameEnderalSE::settings() const
{
  return QList<PluginSetting>();
}

void GameEnderalSE::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Enderal Special Edition", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Enderal Special Edition", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
      || !QFileInfo(myGamesPath() + "/Enderal.ini").exists()) {

      //there is no default ini, actually they are going to put them in for us!
      copyToProfile(gameDirectory().absolutePath(), path, "enderal_default.ini", "Enderal.ini");
      copyToProfile(gameDirectory().absolutePath(), path, "enderalprefs_default.ini", "EnderalPrefs.ini");
    }
    else {
      copyToProfile(myGamesPath(), path, "Enderal.ini");
      copyToProfile(myGamesPath(), path, "EnderalPrefs.ini");
    }
  }
}

QString GameEnderalSE::savegameExtension() const
{
  return "ess";
}

QString GameEnderalSE::savegameSEExtension() const
{
  return "skse";
}

std::shared_ptr<const GamebryoSaveGame> GameEnderalSE::makeSaveGame(QString filePath) const
{
  return std::make_shared<const EnderalSESaveGame>(filePath, this);
}

QString GameEnderalSE::steamAPPId() const
{
  return "976620";
}

QStringList GameEnderalSE::primaryPlugins() const
{
  return {
    "skyrim.esm",
    "update.esm",
    "dawnguard.esm",
    "hearthfires.esm",
    "dragonborn.esm",

    // these two plugins are considered "primary" for users but are not
    // automatically loaded by the game so we need to force-write them
    // to the plugin list
    "enderal - forgotten stories.esm",
    "skyui_se.esp"
  };
}

QStringList GameEnderalSE::DLCPlugins() const
{
  return { };
}


QStringList GameEnderalSE::gameVariants() const
{
  return{ "Regular" };
}

QString GameEnderalSE::gameShortName() const
{
  return "EnderalSE";
}

QStringList GameEnderalSE::validShortNames() const
{
  return { "Skyrim", "SkyrimSE", "Enderal" };
}

QString GameEnderalSE::gameNexusName() const
{
  return "enderalspecialedition";
}

QStringList GameEnderalSE::iniFiles() const
{
  return { "Enderal.ini", "EnderalPrefs.ini" };
}
QStringList GameEnderalSE::CCPlugins() const
{
  QStringList plugins;
  std::set<QString> pluginsLookup;

  const QString path = gameDirectory().filePath("Skyrim.ccc");

  MOBase::forEachLineInFile(path, [&](QString s) {
    const auto lc = s.toLower();
    if (!pluginsLookup.contains(lc)) {
      pluginsLookup.insert(lc);
      plugins.append(std::move(s));
    }
    });

  return plugins;
}

MOBase::IPluginGame::SortMechanism GameEnderalSE::sortMechanism() const
{
  return SortMechanism::NONE;
}

IPluginGame::LoadOrderMechanism GameEnderalSE::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameEnderalSE::nexusModOrganizerID() const
{
  return 0;
}

int GameEnderalSE::nexusGameID() const
{
  return 3685;
}

// Not to delete all the spaces...
MappingType GameEnderalSE::mappings() const
{
  MappingType result;

  for (const QString &profileFile : { "plugins.txt", "loadorder.txt" }) {
    result.push_back({ m_Organizer->profilePath() + "/" + profileFile,
        localAppFolder() + "/Enderal Special Edition/" + profileFile,
        false });
  }

  return result;
}

