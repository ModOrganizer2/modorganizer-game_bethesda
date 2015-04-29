#include "gameskyrim.h"
#include <scopeguard.h>
#include <pluginsetting.h>
#include <igameinfo.h>
#include <executableinfo.h>
#include <utility.h>
#include <memory>
#include <QStandardPaths>


using namespace MOBase;


GameSkyrim::GameSkyrim()
{
}

bool GameSkyrim::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  m_ScriptExtender = std::shared_ptr<ScriptExtender>(new SkyrimScriptExtender());
  m_DataArchives = std::shared_ptr<DataArchives>(new SkyrimDataArchives());
  m_BSAInvalidation = std::shared_ptr<BSAInvalidation>(new SkyrimBSAInvalidation(m_DataArchives, moInfo));
  return true;
}

QString GameSkyrim::identifyGamePath() const
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\Bethesda Softworks\\Skyrim", L"Installed Path");
}

QString GameSkyrim::gameName() const
{
  return "Skyrim";
}

QString GameSkyrim::localAppFolder() const
{
  QString result = getKnownFolderPath(FOLDERID_LocalAppData, false);
  if (result.isEmpty()) {
    // fallback: try the registry
    result = getSpecialPath("Local AppData");
  }

  return result;
}

QString GameSkyrim::myGamesFolderName() const
{
  return "Skyrim";
}



QList<ExecutableInfo> GameSkyrim::executables()
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("SKSE", findInGameFolder("skse_loader.exe"))
      << ExecutableInfo("SBW", findInGameFolder("SBW.exe"))
      << ExecutableInfo("Skyrim", findInGameFolder("TESV.exe"))
      << ExecutableInfo("Skyrim Launcher", findInGameFolder("SkyrimLauncher.exe"))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\LOOT", L"Installed Path"))
      << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe")).withSteamAppId("202480")
  ;
}

QString GameSkyrim::name() const
{
  return "Skyrim Support Plugin";
}

QString GameSkyrim::author() const
{
  return "Tannin";
}

QString GameSkyrim::description() const
{
  return tr("Adds support for the game Skyrim");
}

MOBase::VersionInfo GameSkyrim::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool GameSkyrim::isActive() const
{
  return true;
}

QList<PluginSetting> GameSkyrim::settings() const
{
  return QList<PluginSetting>();
}



void GameSkyrim::copyToProfile(const QString &sourcePath, const QDir &destinationDirectory,
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

void GameSkyrim::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Skyrim", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Skyrim", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/skyrim.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "skyrim_default.ini", "skyrim.ini");
    } else {
      copyToProfile(myGamesPath(), path, "skyrim.ini");
    }

    copyToProfile(myGamesPath(), path, "skyrimprefs.ini");
  }
}

QString GameSkyrim::savegameExtension() const
{
  return "ess";
}

QString GameSkyrim::steamAPPId() const
{
  return "72850";
}

QStringList GameSkyrim::getPrimaryPlugins()
{
  return QStringList({ QString("skyrim.esm"), QString("update.esm") });
}

QIcon GameSkyrim::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath("TESV.exe"));
}

const std::map<std::type_index, boost::any> &GameSkyrim::featureList() const
{
  static std::map<std::type_index, boost::any> result {
    { typeid(BSAInvalidation), m_BSAInvalidation.get() },
    { typeid(ScriptExtender), m_ScriptExtender.get() },
    { typeid(DataArchives), m_DataArchives.get() }
  };

  return result;
}
