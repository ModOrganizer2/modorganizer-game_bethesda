#include "gameoblivion.h"

#include "oblivionbsainvalidation.h"
#include "obliviondataarchives.h"
#include "oblivionsavegameinfo.h"
#include "oblivionscriptextender.h"

#include "pluginsetting.h"
#include "executableinfo.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <memory>

using namespace MOBase;

GameOblivion::GameOblivion()
{
}

bool GameOblivion::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  m_ScriptExtender = std::shared_ptr<ScriptExtender>(new OblivionScriptExtender(this));
  m_DataArchives = std::shared_ptr<DataArchives>(new OblivionDataArchives());
  m_BSAInvalidation = std::shared_ptr<BSAInvalidation>(new OblivionBSAInvalidation(m_DataArchives, this));
  m_SaveGameInfo = std::shared_ptr<SaveGameInfo>(new OblivionSaveGameInfo(this));
  return true;
}

QString GameOblivion::gameName() const
{
  return "Oblivion";
}

QList<ExecutableInfo> GameOblivion::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("OBSE", findInGameFolder(m_ScriptExtender->loaderName()))
      << ExecutableInfo("Oblivion", findInGameFolder(getBinaryName()))
      << ExecutableInfo("Oblivion Launcher", findInGameFolder(getLauncherName()))
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
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameOblivion::settings() const
{
  return QList<PluginSetting>();
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

QStringList GameOblivion::getPrimaryPlugins() const
{
  return { "oblivion.esm", "update.esm" };
}

QString GameOblivion::getGameShortName() const
{
  return "Oblivion";
}

QStringList GameOblivion::getIniFiles() const
{
  return { "oblivion.ini", "oblivionprefs.ini" };
}

QStringList GameOblivion::getDLCPlugins() const
{
  return { "DLCShiveringIsles.esp", "Knights.esp", "DLCFrostcrag.esp",
           "DLCSpellTomes.esp", "DLCMehrunesRazor.esp", "DLCOrrery.esp",
           "DLCThievesDen.esp", "DLCVileLair.esp", "DLCHorseArmor.esp" };
}


int GameOblivion::getNexusModOrganizerID() const
{
  return 38277;
}

int GameOblivion::getNexusGameID() const
{
  return 101;
}
