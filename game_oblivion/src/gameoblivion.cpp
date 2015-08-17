#include "gameoblivion.h"
#include <scopeguard.h>
#include <pluginsetting.h>
#include <igameinfo.h>
#include <executableinfo.h>
#include <utility.h>
#include <memory>
#include <QStandardPaths>


using namespace MOBase;


GameOblivion::GameOblivion()
{
}

bool GameOblivion::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  m_ScriptExtender = std::shared_ptr<ScriptExtender>(new OblivionScriptExtender());
  m_DataArchives = std::shared_ptr<DataArchives>(new OblivionDataArchives());
  m_BSAInvalidation = std::shared_ptr<BSAInvalidation>(new OblivionBSAInvalidation(m_DataArchives, moInfo));
  return true;
}

QString GameOblivion::identifyGamePath() const
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\Bethesda Softworks\\Oblivion", L"Installed Path");
}

QString GameOblivion::gameName() const
{
  return "Oblivion";
}

QString GameOblivion::localAppFolder() const
{
  QString result = getKnownFolderPath(FOLDERID_LocalAppData, false);
  if (result.isEmpty()) {
    // fallback: try the registry
    result = getSpecialPath("Local AppData");
  }

  return result;
}

QString GameOblivion::myGamesFolderName() const
{
  return "Oblivion";
}



QList<ExecutableInfo> GameOblivion::executables()
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("OBSE", findInGameFolder("obse_loader.exe"))
      << ExecutableInfo("Oblivion", findInGameFolder("oblivion.exe"))
      << ExecutableInfo("Oblivion Launcher", findInGameFolder("OblivionLauncher.exe"))
      << ExecutableInfo("Oblivion Mod Manager", findInGameFolder("OblivionModManager.exe"))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", getLootPath())
      << ExecutableInfo("Construction Set", findInGameFolder("TESConstructionSet.exe"))
  ;
}

QString GameOblivion::name() const
{
  return "Oblivion Support Plugin";
}

QString GameOblivion::author() const
{
  return "Tannin";
}

QString GameOblivion::description() const
{
  return tr("Adds support for the game Oblivion");
}

MOBase::VersionInfo GameOblivion::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool GameOblivion::isActive() const
{
  return true;
}

QList<PluginSetting> GameOblivion::settings() const
{
  return QList<PluginSetting>();
}



void GameOblivion::copyToProfile(const QString &sourcePath, const QDir &destinationDirectory,
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

void GameOblivion::initializeProfile(const QDir &path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + "/Oblivion", path, "plugins.txt");
    copyToProfile(localAppFolder() + "/Oblivion", path, "loadorder.txt");
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/oblivion.ini").exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, "oblivion_default.ini", "oblivion.ini");
    } else {
      copyToProfile(myGamesPath(), path, "oblivion.ini");
    }

    copyToProfile(myGamesPath(), path, "oblivionprefs.ini");
  }
}

QString GameOblivion::savegameExtension() const
{
  return "ess";
}

QString GameOblivion::steamAPPId() const
{
  return "22330";
}

QStringList GameOblivion::getPrimaryPlugins()
{
  return { "oblivion.esm", "update.esm" };
}

QIcon GameOblivion::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath("Oblivion.exe"));
}

const std::map<std::type_index, boost::any> &GameOblivion::featureList() const
{
  static std::map<std::type_index, boost::any> result {
    { typeid(BSAInvalidation), m_BSAInvalidation.get() },
    { typeid(ScriptExtender), m_ScriptExtender.get() },
    { typeid(DataArchives), m_DataArchives.get() }
  };

  return result;
}
