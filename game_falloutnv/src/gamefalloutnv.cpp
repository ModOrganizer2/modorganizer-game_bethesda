#include "gameFalloutNV.h"

#include "falloutnvbsainvalidation.h"
#include "falloutnvdataarchives.h"
#include "falloutnvsavegameinfo.h"
#include "falloutnvscriptextender.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "utility.h"
#include "versioninfo.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <winreg.h>

#include <memory>

using namespace MOBase;


GameFalloutNV::GameFalloutNV()
{
}

bool GameFalloutNV::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  m_ScriptExtender = std::shared_ptr<ScriptExtender>(new FalloutNVScriptExtender(this));
  m_DataArchives = std::shared_ptr<DataArchives>(new FalloutNVDataArchives());
  m_BSAInvalidation = std::shared_ptr<BSAInvalidation>(new FalloutNVBSAInvalidation(m_DataArchives, this));
  m_SaveGameInfo = std::shared_ptr<SaveGameInfo>(new FalloutNVSaveGameInfo(this));
  return true;
}

QString GameFalloutNV::identifyGamePath() const
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\Bethesda Softworks\\FalloutNV", L"Installed Path");
}

QString GameFalloutNV::gameName() const
{
  return "New Vegas";
}

QString GameFalloutNV::myGamesFolderName() const
{
  return "FalloutNV";
}

QList<ExecutableInfo> GameFalloutNV::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("NVSE", findInGameFolder(m_ScriptExtender->loaderName()))
      << ExecutableInfo("New Vegas", findInGameFolder(getBinaryName()))
      << ExecutableInfo("Fallout Mod Manager", findInGameFolder("fomm/fomm.exe"))
      << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
      << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", getLootPath())
         ;
}

QString GameFalloutNV::name() const
{
  return "FalloutNV Support Plugin";
}

QString GameFalloutNV::author() const
{
  return "Tannin";
}

QString GameFalloutNV::description() const
{
  return tr("Adds support for the game Fallout New Vegas");
}

MOBase::VersionInfo GameFalloutNV::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool GameFalloutNV::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameFalloutNV::settings() const
{
  return QList<PluginSetting>();
}



void GameFalloutNV::copyToProfile(const QString &sourcePath, const QDir &destinationDirectory,
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

void GameFalloutNV::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/FalloutNV", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/FalloutNV", path, "loadorder.txt");
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

QString GameFalloutNV::savegameExtension() const
{
  return "fos";
}

QString GameFalloutNV::steamAPPId() const
{
  return "22380";
}

QStringList GameFalloutNV::getPrimaryPlugins() const
{
  return { "falloutnv.esm" };
}

QString GameFalloutNV::getGameShortName() const
{
  return "FalloutNV";
}

QStringList GameFalloutNV::getIniFiles() const
{
  return { "fallout.ini", "falloutprefs.ini" };
}

QStringList GameFalloutNV::getDLCPlugins() const
{
  return { "DeadMoney.esm", "HonestHearts.esm", "OldWorldBlues.esm",
           "LonesomeRoad.esm", "GunRunnersArsenal.esm", "CaravanPack.esm",
           "ClassicPack.esm", "MercenaryPack.esm", "TribalPack.esm" };
}

int GameFalloutNV::getNexusModOrganizerID() const
{
  return 42572;
}

int GameFalloutNV::getNexusGameID() const
{
  return 130;
}
