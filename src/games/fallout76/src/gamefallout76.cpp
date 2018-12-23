#include "gameFallout76.h"

#include "fallout76dataarchives.h"
#include "fallout76scriptextender.h"
#include "fallout76savegameinfo.h"
#include "fallout76unmanagedmods.h"

#include <pluginsetting.h>
#include <executableinfo.h>
#include <gamebryolocalsavegames.h>
#include <creationgameplugins.h>
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

GameFallout76::GameFallout76()
{
}

bool GameFallout76::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature<ScriptExtender>(new Fallout76ScriptExtender(this));
  registerFeature<DataArchives>(new Fallout76DataArchives(myGamesPath()));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "Fallout76.ini"));
  registerFeature<SaveGameInfo>(new Fallout76SaveGameInfo(this));
  registerFeature<GamePlugins>(new CreationGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new Fallout76UnmangedMods(this));

  return true;
}

QString GameFallout76::gameName() const
{
  return "Fallout 76";
}

QList<ExecutableInfo> GameFallout76::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("F76SE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
      << ExecutableInfo("Fallout 76", findInGameFolder(binaryName()))
      << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
      << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Fallout76\"")
         ;
}

QString GameFallout76::name() const
{
  return "Fallout 76 Support Plugin";
}

QString GameFallout76::author() const
{
  return "EntranceJew";
}

QString GameFallout76::description() const
{
  return tr("Adds support for the game Fallout 76.\n"
            "Splash by %1").arg("nekoyoubi");
}

MOBase::VersionInfo GameFallout76::version() const
{
  return VersionInfo(1, 3, 0, VersionInfo::RELEASE_FINAL);
}

bool GameFallout76::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameFallout76::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout76::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Fallout76", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Fallout76", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/fallout76.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "fallout76_default.ini", "fallout76.ini");
    } else {
      copyToProfile(myGamesPath(), path, "fallout76.ini");
    }

    copyToProfile(myGamesPath(), path, "fallout76prefs.ini");
	copyToProfile(myGamesPath(), path, "fallout76custom.ini");
  }
}

QString GameFallout76::savegameExtension() const
{
  return "bgs";
}

QString GameFallout76::savegameSEExtension() const
{
  return "f76se";
}

QString GameFallout76::steamAPPId() const
{
  return "n/a";
}

QStringList GameFallout76::primaryPlugins() const {
  QStringList plugins = {"SeventySix.esm"};

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameFallout76::gameVariants() const
{
  return { "Regular" };
}

QString GameFallout76::gameShortName() const
{
  return "Fallout76";
}

QString GameFallout76::gameNexusName() const
{
  return "Fallout76";
}

QStringList GameFallout76::iniFiles() const
{
    return { "Fallout76.ini", "Fallout76Prefs.ini", "Fallout76Custom.ini" };
}

QStringList GameFallout76::DLCPlugins() const
{
  return {};
}

QStringList GameFallout76::CCPlugins() const
{
  QStringList plugins = {};
  QFile file(gameDirectory().absoluteFilePath("Fallout76.ccc"));
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

IPluginGame::LoadOrderMechanism GameFallout76::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameFallout76::nexusModOrganizerID() const
{
  return -1;
}

int GameFallout76::nexusGameID() const
{
  return 2590;
}
