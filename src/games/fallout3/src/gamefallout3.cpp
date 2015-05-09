#include "gameFallout3.h"
#include <scopeguard.h>
#include <pluginsetting.h>
#include <igameinfo.h>
#include <executableinfo.h>
#include <utility.h>
#include <memory>
#include <QStandardPaths>


using namespace MOBase;


GameFallout3::GameFallout3()
{
}

bool GameFallout3::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  m_ScriptExtender = std::shared_ptr<ScriptExtender>(new Fallout3ScriptExtender());
  m_DataArchives = std::shared_ptr<DataArchives>(new Fallout3DataArchives());
  m_BSAInvalidation = std::shared_ptr<BSAInvalidation>(new Fallout3BSAInvalidation(m_DataArchives, moInfo));
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

QString GameFallout3::localAppFolder() const
{
  QString result = getKnownFolderPath(FOLDERID_LocalAppData, false);
  if (result.isEmpty()) {
    // fallback: try the registry
    result = getSpecialPath("Local AppData");
  }

  return result;
}

QString GameFallout3::myGamesFolderName() const
{
  return "Fallout3";
}

QList<ExecutableInfo> GameFallout3::executables()
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("FOSE", findInGameFolder("fose_loader.exe"))
      << ExecutableInfo("Fallout 3", findInGameFolder("Fallout3.exe"))
      << ExecutableInfo("Fallout Mod Manager", findInGameFolder("fomm/fomm.exe"))
      << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
      << ExecutableInfo("Fallout Launcher", findInGameFolder("Fallout3Launcher.exe"))
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
  return "22380";
}

QStringList GameFallout3::getPrimaryPlugins()
{
  return { "fallout3.esm" };
}

QIcon GameFallout3::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath("Fallout3.exe"));
}

const std::map<std::type_index, boost::any> &GameFallout3::featureList() const
{
  static std::map<std::type_index, boost::any> result {
    { typeid(BSAInvalidation), m_BSAInvalidation.get() },
    { typeid(ScriptExtender), m_ScriptExtender.get() },
    { typeid(DataArchives), m_DataArchives.get() }
  };

  return result;
}
