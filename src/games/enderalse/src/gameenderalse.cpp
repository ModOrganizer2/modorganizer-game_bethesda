#include "gameenderalse.h"

#include "enderalsedataarchives.h"
#include "enderalsegameplugins.h"
#include "enderalselocalsavegames.h"
#include "enderalsemoddatachecker.h"
#include "enderalsemoddatacontent.h"
#include "enderalsesavegame.h"
#include "enderalsescriptextender.h"
#include "enderalseunmanagedmods.h"
#include "steamutility.h"

#include "versioninfo.h"
#include <executableinfo.h>
#include <gamebryosavegameinfo.h>
#include <ipluginlist.h>
#include <pluginsetting.h>
#include <utility.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include "scopeguard.h"
#include <memory>

using namespace MOBase;

GameEnderalSE::GameEnderalSE() {}

void GameEnderalSE::setVariant(QString variant)
{
  m_GameVariant = variant;
}

void GameEnderalSE::checkVariants()
{
  QFileInfo gog_dll(m_GamePath + "\\Galaxy64.dll");
  if (gog_dll.exists())
    setVariant("GOG");
  else
    setVariant("Steam");
}

QDir GameEnderalSE::documentsDirectory() const
{
  return m_MyGamesPath;
}

void GameEnderalSE::detectGame()
{
  m_GamePath = identifyGamePath();
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QString GameEnderalSE::identifyGamePath() const
{
  QMap<QString, QString> paths = {
      {"Software\\Bethesda Softworks\\" + gameName(), "Installed Path"},
      {"Software\\GOG.com\\Games\\1708684988", "path"},
  };
  QString result;
  try {
    for (auto& path : paths.toStdMap()) {
      result = findInRegistry(HKEY_LOCAL_MACHINE, path.first.toStdWString().c_str(),
                              path.second.toStdWString().c_str());
      if (!result.isEmpty())
        break;
    }
  } catch (MOBase::MyException) {
    result = MOBase::findSteamGame("Enderal Special Edition",
                                   "Data\\Enderal - Forgotten Stories.esm");
  }
  return result;
}

void GameEnderalSE::setGamePath(const QString& path)
{
  m_GamePath = path;
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
  registerFeature<DataArchives>(new EnderalSEDataArchives(myGamesPath()));
  registerFeature<LocalSavegames>(
      new EnderalSELocalSavegames(myGamesPath(), "Enderal.ini"));
}

QDir GameEnderalSE::savesDirectory() const
{
  return QDir(m_MyGamesPath + "/Saves");
}

QString GameEnderalSE::myGamesPath() const
{
  return m_MyGamesPath;
}

bool GameEnderalSE::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

bool GameEnderalSE::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature<ScriptExtender>(new EnderalSEScriptExtender(this));
  registerFeature<DataArchives>(new EnderalSEDataArchives(myGamesPath()));
  registerFeature<LocalSavegames>(
      new EnderalSELocalSavegames(myGamesPath(), "Enderal.ini"));
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

QString GameEnderalSE::gameDirectoryName() const
{
  if (selectedVariant() == "GOG")
    return "Enderal Special Edition GOG";
  else
    return "Enderal Special Edition";
}

QIcon GameEnderalSE::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(getLauncherName()));
}

QList<ExecutableInfo> GameEnderalSE::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Enderal Special Edition (SKSE)",
                           findInGameFolder(feature<ScriptExtender>()->loaderName()))
         << ExecutableInfo("Enderal Special Edition Launcher",
                           findInGameFolder(getLauncherName()))
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("1946180")
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Enderal Special Edition\"");
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

QFileInfo GameEnderalSE::findInGameFolder(const QString& relativePath) const
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
  return VersionInfo(1, 1, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameEnderalSE::settings() const
{
  return QList<PluginSetting>();
}

void GameEnderalSE::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + gameDirectoryName(), path, "plugins.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo(myGamesPath() + "/Enderal.ini").exists()) {

      // there is no default ini, actually they are going to put them in for us!
      copyToProfile(gameDirectory().absolutePath(), path, "enderal_default.ini",
                    "Enderal.ini");
      copyToProfile(gameDirectory().absolutePath(), path, "enderalprefs_default.ini",
                    "EnderalPrefs.ini");
    } else {
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

std::shared_ptr<const GamebryoSaveGame>
GameEnderalSE::makeSaveGame(QString filePath) const
{
  return std::make_shared<const EnderalSESaveGame>(filePath, this);
}

QString GameEnderalSE::steamAPPId() const
{
  if (selectedVariant() == "Steam")
    return "976620";
  return "";
}

QStringList GameEnderalSE::primaryPlugins() const
{
  return {"skyrim.esm", "update.esm", "dawnguard.esm", "hearthfires.esm",
          "dragonborn.esm",

          // these two plugins are considered "primary" for users but are not
          // automatically loaded by the game so we need to force-write them
          // to the plugin list
          "enderal - forgotten stories.esm", "skyui_se.esp"};
}

QStringList GameEnderalSE::DLCPlugins() const
{
  return {};
}

QStringList GameEnderalSE::gameVariants() const
{
  return {"Steam", "GOG"};
}

QString GameEnderalSE::gameShortName() const
{
  return "EnderalSE";
}

QStringList GameEnderalSE::validShortNames() const
{
  return {"Skyrim", "SkyrimSE", "Enderal"};
}

QString GameEnderalSE::gameNexusName() const
{
  return "enderalspecialedition";
}

QStringList GameEnderalSE::iniFiles() const
{
  return {"Enderal.ini", "EnderalPrefs.ini"};
}

QStringList GameEnderalSE::CCPlugins() const
{
  return {};
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

QDir GameEnderalSE::gameDirectory() const
{
  return QDir(m_GamePath);
}

// Not to delete all the spaces...
MappingType GameEnderalSE::mappings() const
{
  MappingType result;

  for (const QString& profileFile : {"plugins.txt", "loadorder.txt"}) {
    result.push_back({m_Organizer->profilePath() + "/" + profileFile,
                      localAppFolder() + "/" + gameDirectoryName() + "/" + profileFile,
                      false});
  }

  return result;
}
