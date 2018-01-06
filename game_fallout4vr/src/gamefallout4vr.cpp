#include "gameFallout4vr.h"

#include "fallout4vrdataarchives.h"
#include "fallout4vrscriptextender.h"
#include "fallout4vrsavegameinfo.h"
#include "fallout4vrgameplugins.h"
#include "fallout4vrunmanagedmods.h"

#include <pluginsetting.h>
#include "iplugingame.h"
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

#include "utility.h"
#include <windows.h>
#include <winreg.h>
#include "scopeguard.h"

using namespace MOBase;

GameFallout4VR::GameFallout4VR()
{
}

bool GameFallout4VR::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature<ScriptExtender>(new Fallout4VRScriptExtender(this));
  registerFeature<DataArchives>(new Fallout4VRDataArchives(myGamesPath()));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "fallout4custom.ini"));
  registerFeature<SaveGameInfo>(new Fallout4VRSaveGameInfo(this));
  registerFeature<GamePlugins>(new Fallout4VRGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new Fallout4VRUnmangedMods(this));

  return true;
}

QString GameFallout4VR::gameName() const
{
  return "Fallout 4 VR";
}

QList<ExecutableInfo> GameFallout4VR::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("F4SE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
      << ExecutableInfo("Fallout 4 VR", findInGameFolder(binaryName()))
      //<< ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName())) // Fallout 4 VR does not have a launcher
      << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
      << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Fallout4\"")
         ;
}

QString GameFallout4VR::name() const
{
  return "Fallout4VR Support Plugin";
}

QString GameFallout4VR::author() const
{
  return "Tannin";
}

QString GameFallout4VR::description() const
{
  return tr("Adds support for the game Fallout 4 VR.\n"
            "Splash by %1").arg("nekoyoubi");
}

MOBase::VersionInfo GameFallout4VR::version() const
{
  return VersionInfo(0, 3, 0, VersionInfo::RELEASE_BETA);
}

bool GameFallout4VR::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameFallout4VR::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout4VR::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Fallout4VR", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Fallout4VR", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    /* 
       There is a fallout4.ini in the game installation directory, but it never get copied to "My Games/Fallout4VR".
       The only files in the MyGames directory are fallout4custom.ini, fallout4prefs.ini and fallout4vrcustom.ini.
       All settings you would expect in the fallout4.ini can be put into the fallout4custom.ini.
    */
    /*if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/fallout4.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "fallout4.ini");
    } else {
      copyToProfile(myGamesPath(), path, "fallout4.ini");
    }*/

	copyToProfile(myGamesPath(), path, "fallout4custom.ini");
    copyToProfile(myGamesPath(), path, "fallout4prefs.ini");
	copyToProfile(myGamesPath(), path, "fallout4vrcustom.ini");
  }
}

QString GameFallout4VR::savegameExtension() const
{
  return "fos";
}

QString GameFallout4VR::savegameSEExtension() const
{
  return "f4se";
}

QString GameFallout4VR::steamAPPId() const
{
  return "611660";
}

QStringList GameFallout4VR::primaryPlugins() const {
  /*QStringList plugins = {"fallout4.esm", "fallout4_vr.esm", "dlcrobot.esm", "dlcworkshop01.esm", "dlccoast.esm", "dlcworkshop02.esm",
          "dlcworkshop03.esm", "dlcnukaworld.esm"};*/
  // Fallout 4 VR does not support the DLCs, so we need to tread them as unmanaged plugins.
  QStringList plugins = {"fallout4.esm", "fallout4_vr.esm"};

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameFallout4VR::gameVariants() const
{
  return { "Regular" };
}

QString GameFallout4VR::gameShortName() const
{
  return "Fallout4VR";
}

QString GameFallout4VR::gameNexusName() const
{
  return "Fallout4";
}

QStringList GameFallout4VR::iniFiles() const
{
    return { "fallout4prefs.ini", "fallout4custom.ini", "fallout4vrcustom.ini" };
}

QStringList GameFallout4VR::DLCPlugins() const
{
  return {"dlcrobot.esm", "dlcworkshop01.esm", "dlccoast.esm", "dlcworkshop02.esm", "dlcworkshop03.esm",
	      "dlcnukaworld.esm", "dlcultrahighresolution.esm"};
}

QStringList GameFallout4VR::CCPlugins() const
{
  QStringList plugins = {};
  QFile file(gameDirectory().absoluteFilePath("Fallout4.ccc"));
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

IPluginGame::LoadOrderMechanism GameFallout4VR::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameFallout4VR::nexusModOrganizerID() const
{
  return 0; //...
}

int GameFallout4VR::nexusGameID() const
{
  return 1151;
}

QString GameFallout4VR::getLauncherName() const
{
  return binaryName(); // Fallout 4 VR has no Launcher, so we just return the name of the game binary
}
