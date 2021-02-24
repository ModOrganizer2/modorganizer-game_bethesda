#include "gameenderalse.h"

#include "enderalsebsainvalidation.h"
#include "enderalsedataarchives.h"
#include "enderalsescriptextender.h"
#include "enderalseunmanagedmods.h"
#include "enderalsemoddatachecker.h"
#include "enderalsemoddatacontent.h"
#include "enderalsesavegame.h"

#include <pluginsetting.h>
#include <executableinfo.h>
#include <gamebryosavegameinfo.h>
#include <gamebryolocalsavegames.h>
#include <creationgameplugins.h>
#include "versioninfo.h"
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

void GameEnderalSE::setGamePath(const QString &path)
{
  m_GamePath = path;
}

QDir GameEnderalSE::documentsDirectory() const
{
  return m_MyGamesPath;
}

QString GameEnderalSE::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\Skyrim Special Edition";
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(), L"Installed Path");
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

void GameEnderalSE::detectGame()
{
  m_GamePath = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath("Skyrim Special Edition");
}

bool GameEnderalSE::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature<ScriptExtender>(new EnderalSEScriptExtender(this));
  registerFeature<DataArchives>(new EnderalSEDataArchives(myGamesPath()));
  registerFeature<BSAInvalidation>(new EnderalSEBSAInvalidation(feature<DataArchives>(), this));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "skyrimcustom.ini"));
  registerFeature<ModDataChecker>(new EnderalSEModDataChecker(this));
  registerFeature<ModDataContent>(new EnderalSEModDataContent(this));
  registerFeature<SaveGameInfo>(new GamebryoSaveGameInfo(this));
  registerFeature<GamePlugins>(new CreationGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new EnderalSEUnmangedMods(this));

  return true;
}

QString GameEnderalSE::gameName() const
{
  return "Enderal Special Edition";
}

QList<ExecutableInfo> GameEnderalSE::executables() const
{
  return QList<ExecutableInfo>()
    << ExecutableInfo("Enderal Special Edition (SKSE)", findInGameFolder(feature<ScriptExtender>()->loaderName()))
    // << ExecutableInfo("Enderal Special Edition Launcher", findInGameFolder(getLauncherName()))
    // << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Skyrim Special Edition\"")
    << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
    ;
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
  return "";
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
  return "Holt59 & Archost & ZachHaber";
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
      || !QFileInfo(myGamesPath() + "/skyrim.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "skyrim_default.ini", "skyrim.ini");
    }
    else {
      copyToProfile(myGamesPath(), path, "skyrim.ini");
    }

    copyToProfile(myGamesPath(), path, "skyrimprefs.ini");
    copyToProfile(myGamesPath(), path, "skyrimcustom.ini");
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
    "Enderal - Forgotten Stories.esm",
    "update.esm"
  };
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
  return{ "skyrim.ini", "skyrimprefs.ini", "skyrimcustom.ini" };
}

QStringList GameEnderalSE::DLCPlugins() const
{
  return {};
}

QStringList GameEnderalSE::CCPlugins() const
{
  return {};
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

QDir GameEnderalSE::gameDirectory() const
{
  return QDir(m_GamePath);
}

// Not to delete all the spaces...
MappingType GameEnderalSE::mappings() const
{
  MappingType result;

  for (const QString &profileFile : { "plugins.txt", "loadorder.txt" }) {
    result.push_back({ m_Organizer->profilePath() + "/" + profileFile,
        localAppFolder() + "/" + gameName() + "/" + profileFile,
        false });
  }

  return result;
}

