#include "gameskyrimvr.h"

#include "skyrimvrdataarchives.h"
#include "skyrimvrscriptextender.h"
#include "skyrimvrsavegameinfo.h"
#include "skyrimvrgameplugins.h"
#include "skyrimvrunmanagedmods.h"

#include <pluginsetting.h>
#include <executableinfo.h>
#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include "versioninfo.h"

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

GameSkyrimVR::GameSkyrimVR()
{
}

void GameSkyrimVR::setGamePath(const QString &path)
{
  m_GamePath = path;
}

QDir GameSkyrimVR::documentsDirectory() const
{
  return m_MyGamesPath;
}

QString GameSkyrimVR::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\" + gameName();
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(), L"Installed Path");
}

QDir GameSkyrimVR::savesDirectory() const
{
  return QDir(m_MyGamesPath + "/Saves");
}

QString GameSkyrimVR::myGamesPath() const
{
  return m_MyGamesPath;
}

bool GameSkyrimVR::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

bool GameSkyrimVR::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  m_GamePath = GameSkyrimVR::identifyGamePath();
  m_MyGamesPath = determineMyGamesPath(gameName());

  registerFeature<ScriptExtender>(new SkyrimVRScriptExtender(this));
  registerFeature<DataArchives>(new SkyrimVRDataArchives(myGamesPath()));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "SkyrimVR.ini"));
  registerFeature<SaveGameInfo>(new SkyrimVRSaveGameInfo(this));
  registerFeature<GamePlugins>(new SkyrimVRGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new SkyrimVRUnmangedMods(this));

  return true;
}



QString GameSkyrimVR::gameName() const
{
  return "Skyrim VR";
}

QList<ExecutableInfo> GameSkyrimVR::executables() const
{
  return QList<ExecutableInfo>()
    << ExecutableInfo("SKSE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
    << ExecutableInfo("Skyrim VR", findInGameFolder(binaryName()))
    << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
    //<< ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Skyrim Special Edition\"") Let's not make an entry for a different game
    ;
}

QFileInfo GameSkyrimVR::findInGameFolder(const QString &relativePath) const
{
  return QFileInfo(m_GamePath + "/" + relativePath);
}

QString GameSkyrimVR::name() const
{
  return "Skyrim VR Support Plugin";
}

QString GameSkyrimVR::author() const
{
  return "Brixified & MO2 Team";
}

QString GameSkyrimVR::description() const
{
  return tr("Adds support for the game Skyrim VR.");
}

MOBase::VersionInfo GameSkyrimVR::version() const
{
  return VersionInfo(0, 2, 0, VersionInfo::RELEASE_CANDIDATE);
}

bool GameSkyrimVR::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameSkyrimVR::settings() const
{
  return QList<PluginSetting>();
}

void GameSkyrimVR::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Skyrim VR", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Skyrim VR", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
      || !QFileInfo(myGamesPath() + "/skyrimvr.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "skyrim.ini", "skyrimvr.ini");
    } else {
      copyToProfile(myGamesPath(), path, "skyrimvr.ini");
    }

    copyToProfile(myGamesPath(), path, "skyrimprefs.ini");
  }
}

QString GameSkyrimVR::savegameExtension() const
{
  return "ess";
}

QString GameSkyrimVR::savegameSEExtension() const
{
  return "skse";
}

QString GameSkyrimVR::steamAPPId() const
{
  return "611670";
}

QStringList GameSkyrimVR::primaryPlugins() const {
  QStringList plugins = { "skyrim.esm", "update.esm", "dawnguard.esm", "hearthfires.esm", "dragonborn.esm", "skyrimvr.esm" };

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameSkyrimVR::gameVariants() const
{
  return{ "Regular" };
}

QString GameSkyrimVR::gameShortName() const
{
  return "SkyrimVR";
}

QStringList GameSkyrimVR::primarySources() const
{
  return { "SkyrimSE" };
}

QStringList GameSkyrimVR::validShortNames() const
{
  return { "Skyrim", "SkyrimSE" };
}

QString GameSkyrimVR::gameNexusName() const
{
  return "skyrimspecialedition";
}


QStringList GameSkyrimVR::iniFiles() const
{
  return{ "skyrimvr.ini", "skyrimprefs.ini" };
}

QStringList GameSkyrimVR::DLCPlugins() const
{
  return{ "dawnguard.esm", "hearthfires.esm", "dragonborn.esm" };
}

QStringList GameSkyrimVR::CCPlugins() const
{
  QStringList plugins = {};
  QFile file(gameDirectory().filePath("Skyrim.ccc"));
  if (file.open(QIODevice::ReadOnly)) {
    ON_BLOCK_EXIT([&file]() { file.close(); });

    if (file.size() == 0) {
      return plugins;
    }
    while (!file.atEnd()) {
      QByteArray line = file.readLine().trimmed();
      QString modName;
      if ((line.size() > 0) && (line.at(0) != '#')) {
        modName = QString::fromUtf8(line.constData()).toLower();
      }

      if (modName.size() > 0) {
        if (!plugins.contains(modName, Qt::CaseInsensitive)) {
          plugins.append(modName);
        }
      }
    }
  }
  return plugins;
}

IPluginGame::LoadOrderMechanism GameSkyrimVR::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

MOBase::IPluginGame::SortMechanism GameSkyrimVR::sortMechanism() const
{
  return SortMechanism::NONE;
}

int GameSkyrimVR::nexusModOrganizerID() const
{
  return 6194;
}

int GameSkyrimVR::nexusGameID() const
{
  return 1704;
}

QString GameSkyrimVR::getLauncherName() const
{
  return binaryName(); // Skyrim VR has no Launcher, so we just return the name of the game binary
}

QDir GameSkyrimVR::gameDirectory() const
{
  return QDir(m_GamePath);
}

// Not to delete all the spaces...
MappingType GameSkyrimVR::mappings() const
{
  MappingType result;

  for (const QString &profileFile : { "plugins.txt", "loadorder.txt" }) {
    result.push_back({ m_Organizer->profilePath() + "/" + profileFile,
      localAppFolder() + "/" + gameName() + "/" + profileFile,
      false });
  }

  return result;
}
