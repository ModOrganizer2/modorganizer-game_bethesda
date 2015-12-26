#include "gameFallout3.h"

#include "fallout3bsainvalidation.h"
#include "fallout3scriptextender.h"
#include "fallout3dataarchives.h"
#include "fallout3savegameinfo.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "versioninfo.h"
#include "utility.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QList>
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
  m_ScriptExtender = std::shared_ptr<ScriptExtender>(new Fallout3ScriptExtender(this));
  m_DataArchives = std::shared_ptr<DataArchives>(new Fallout3DataArchives());
  m_BSAInvalidation = std::shared_ptr<BSAInvalidation>(new Fallout3BSAInvalidation(m_DataArchives, this));
  m_SaveGameInfo = std::shared_ptr<SaveGameInfo>(new Fallout3SaveGameInfo(this));
  return true;
}

QString GameFallout3::identifyGamePath() const
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\Bethesda Softworks\\Fallout3", L"Installed Path");
}

QString GameFallout3::gameName() const
{
  return "Fallout 3";
}

QString GameFallout3::myGamesFolderName() const
{
  return "Fallout3";
}

QList<ExecutableInfo> GameFallout3::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("FOSE", findInGameFolder(m_ScriptExtender->loaderName()))
      << ExecutableInfo("Fallout 3", findInGameFolder(getBinaryName()))
      << ExecutableInfo("Fallout Mod Manager", findInGameFolder("fomm/fomm.exe"))
      << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
      << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", getLootPath());
         ;
}

QString GameFallout3::name() const
{
  return "Fallout3 Support Plugin";
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
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool GameFallout3::isActive() const
{
  return true;
}

QList<PluginSetting> GameFallout3::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout3::copyToProfile(const QString &sourcePath, const QDir &destinationDirectory,
                               const QString &sourceFileName, const QString &destinationFileName) const
{
  QString filePath = destinationDirectory.absoluteFilePath(destinationFileName.isEmpty() ? sourceFileName
                                                                                         : destinationFileName);
  if (!QFileInfo(filePath).exists()) {
    if (!shellCopy(sourcePath + "/" + sourceFileName, filePath)) {
      // if copy file fails, create the file empty
      QFile(filePath).open(QIODevice::WriteOnly);
    }
  }
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
  }
}

QString GameFallout3::savegameExtension() const
{
  return "fos";
}

QString GameFallout3::steamAPPId() const
{
  if (selectedVariant() == "Game Of The Year") {
    return "22370";
  } else {
    return "22300";
  }
}

QStringList GameFallout3::getPrimaryPlugins() const
{
  return { "fallout3.esm" };
}


QStringList GameFallout3::gameVariants() const
{
  return { "Regular", "Game Of The Year" };
}

QString GameFallout3::getGameShortName() const
{
  return "Fallout3";
}

QStringList GameFallout3::getIniFiles() const
{
  return { "fallout.ini", "falloutprefs.ini" };
}

QStringList GameFallout3::getDLCPlugins() const
{
  return { "ThePitt.esm", "Anchorage.esm", "BrokenSteel.esm", "PointLookout.esm", "Zeta.esm" };
}

int GameFallout3::getNexusModOrganizerID() const
{
  return 16348;
}

int GameFallout3::getNexusGameID() const
{
  return 120;
}
