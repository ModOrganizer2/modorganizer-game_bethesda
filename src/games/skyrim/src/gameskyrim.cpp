#include "gameskyrim.h"

#include <scopeguard.h>
#include <pluginsetting.h>
#include <executableinfo.h>
#include <utility.h>

#include <QDebug>
#include <QStandardPaths>

#include <Windows.h>

#include <exception>
#include <memory>
#include <vector>


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
  m_BSAInvalidation = std::shared_ptr<BSAInvalidation>(new SkyrimBSAInvalidation(m_DataArchives, this));
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



QList<ExecutableInfo> GameSkyrim::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("SKSE", findInGameFolder("skse_loader.exe"))
      << ExecutableInfo("SBW", findInGameFolder("SBW.exe"))
      << ExecutableInfo("Skyrim", findInGameFolder(getBinaryName()))
      << ExecutableInfo("Skyrim Launcher", findInGameFolder("SkyrimLauncher.exe"))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", getLootPath())
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

QStringList GameSkyrim::getPrimaryPlugins() const
{
  return { "skyrim.esm", "update.esm" };
}

QIcon GameSkyrim::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(getBinaryName()));
}

std::map<std::type_index, boost::any> GameSkyrim::featureList() const
{
  static std::map<std::type_index, boost::any> result {
    { typeid(BSAInvalidation), m_BSAInvalidation.get() },
    { typeid(ScriptExtender), m_ScriptExtender.get() },
    { typeid(DataArchives), m_DataArchives.get() }
  };

  return result;
}

QString GameSkyrim::getBinaryName() const
{
  return "TESV.exe";
}

QString GameSkyrim::getNexusName() const
{
  return "Skyrim";
}

QStringList GameSkyrim::getIniFiles() const
{
  return { "skyrim.ini", "skyrimprefs.ini" };
}

QStringList GameSkyrim::getDLCPlugins() const
{
  return { "Dawnguard.esm", "Dragonborn.esm", "HearthFires.esm",
           "HighResTexturePack01.esp", "HighResTexturePack02.esp", "HighResTexturePack03.esp" };
}

namespace {
//Note: This is ripped off from shared/util. And in an upcoming move, the fomod
//installer requires something similar. I suspect I should abstract this out
//into gamebro and add a getVersion mechanism in gamebryo (or lower level)

VS_FIXEDFILEINFO GetFileVersion(const std::wstring &fileName)
{
  DWORD handle = 0UL;
  DWORD size = ::GetFileVersionInfoSizeW(fileName.c_str(), &handle);
  if (size == 0) {
    throw std::runtime_error("failed to determine file version info size");
  }

  std::vector<char> buffer(size);
  handle = 0UL;
  if (!::GetFileVersionInfoW(fileName.c_str(), handle, size, buffer.data())) {
    throw std::runtime_error("failed to determine file version info");
  }

  void *versionInfoPtr = nullptr;
  UINT versionInfoLength = 0;
  if (!::VerQueryValue(buffer.data(), L"\\", &versionInfoPtr, &versionInfoLength)) {
    throw std::runtime_error("failed to determine file version");
  }

  return *static_cast<VS_FIXEDFILEINFO*>(versionInfoPtr);
}

}

IPluginGame::LoadOrderMechanism GameSkyrim::getLoadOrderMechanism() const
{
  try {
    std::wstring fileName = gameDirectory().absoluteFilePath(getBinaryName()).toStdWString().c_str();
    VS_FIXEDFILEINFO versionInfo = ::GetFileVersion(fileName);
    if ((versionInfo.dwFileVersionMS > 0x10004) || // version >= 1.5.x?
        ((versionInfo.dwFileVersionMS == 0x10004) && (versionInfo.dwFileVersionLS >= 0x1A0000))) { // version >= ?.4.26
      return LoadOrderMechanism::PluginsTxt;
    }
  } catch (const std::exception &e) {
    qCritical() << "TESV.exe is invalid: " << e.what();
  }
  return LoadOrderMechanism::FileTime;
}
