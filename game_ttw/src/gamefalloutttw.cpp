#include "gamefalloutttw.h"

#include "falloutttwbsainvalidation.h"
#include "falloutttwdataarchives.h"
#include "falloutttwsavegameinfo.h"
#include "falloutttwscriptextender.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "iplugingame.h"
#include "versioninfo.h"
#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include <gamebryounmanagedmods.h>

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

namespace {
  std::unique_ptr<BYTE[]> getRegValue(HKEY key, LPCWSTR path, LPCWSTR value,
    DWORD flags, LPDWORD type = nullptr)
  {
    DWORD size = 0;
    HKEY subKey;
    LONG res = ::RegOpenKeyExW(key, path, 0,
      KEY_QUERY_VALUE | KEY_WOW64_32KEY, &subKey);
    if (res != ERROR_SUCCESS) {
      return std::unique_ptr<BYTE[]>();
    }
    res = ::RegGetValueW(subKey, L"", value, flags, type, nullptr, &size);
    if (res == ERROR_FILE_NOT_FOUND || res == ERROR_UNSUPPORTED_TYPE) {
      return std::unique_ptr<BYTE[]>();
    }
    if (res != ERROR_SUCCESS && res != ERROR_MORE_DATA) {
      throw MOBase::MyException(QObject::tr("failed to query registry path (preflight): %1").arg(res, 0, 16));
    }

    std::unique_ptr<BYTE[]> result(new BYTE[size]);
    res = ::RegGetValueW(subKey, L"", value, flags, type, result.get(), &size);

    if (res != ERROR_SUCCESS) {
      throw MOBase::MyException(QObject::tr("failed to query registry path (read): %1").arg(res, 0, 16));
    }

    return result;
  }

  QString findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value)
  {
    std::unique_ptr<BYTE[]> buffer = getRegValue(baseKey, path, value, RRF_RT_REG_SZ | RRF_NOEXPAND);

    return QString::fromUtf16(reinterpret_cast<const ushort*>(buffer.get()));
  }
}

using namespace MOBase;

GameFalloutTTW::GameFalloutTTW()
{
}

bool GameFalloutTTW::init(IOrganizer *moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }
  registerFeature<ScriptExtender>(new FalloutTTWScriptExtender(this));
  registerFeature<DataArchives>(new FalloutTTWDataArchives(myGamesPath()));
  registerFeature<BSAInvalidation>(new FalloutTTWBSAInvalidation(feature<DataArchives>(), this));
  registerFeature<SaveGameInfo>(new FalloutTTWSaveGameInfo(this));
  registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "fallout.ini"));
  registerFeature<GamePlugins>(new GamebryoGamePlugins(moInfo));
  registerFeature<UnmanagedMods>(new GamebryoUnmangedMods(this));
  return true;
}

QString GameFalloutTTW::gameName() const
{
  return "TTW";
}

QList<ExecutableInfo> GameFalloutTTW::executables() const
{
  return QList<ExecutableInfo>()
      << ExecutableInfo("NVSE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
      << ExecutableInfo("Tale of Two Wastelands", findInGameFolder(binaryName()))
      << ExecutableInfo("Fallout Mod Manager", findInGameFolder("fomm/fomm.exe"))
      << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
      << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
      << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
      << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"FalloutNV\"")
         ;
}

QString GameFalloutTTW::name() const
{
  return "FalloutTTW Support Plugin";
}

QString GameFalloutTTW::author() const
{
  return "SuperSandro2000";
}

QString GameFalloutTTW::description() const
{
  return tr("Adds support for the game Fallout TTW");
}

MOBase::VersionInfo GameFalloutTTW::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool GameFalloutTTW::isActive() const
{
  return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameFalloutTTW::settings() const
{
  return QList<PluginSetting>();
}

void GameFalloutTTW::initializeProfile(const QDir &path, ProfileSettings settings) const
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
    copyToProfile(myGamesPath(), path, "falloutcustom.ini");
    copyToProfile(myGamesPath(), path, "custom.ini");
  }
}

QString GameFalloutTTW::savegameExtension() const
{
  return "fos";
}

QString GameFalloutTTW::savegameSEExtension() const
{
  return "nvse";
}

QString GameFalloutTTW::steamAPPId() const
{
  return "22380";
}

QStringList GameFalloutTTW::primaryPlugins() const
{
  return { "falloutnv.esm", "deadmoney.esm", "honesthearts.esm",
           "oldworldblues.esm", "lonesomeroad.esm", "gunrunnersarsenal.esm",
           "fallout3.esm", "anchorage.esm", "thepitt.esm", "brokensteel.esm",
		   "pointlookout.esm", "zeta.esm", "caravanpack.esm", "classicpack.esm",
		   "mercenarypack.esm", "tribalpack.esm", "taleoftwowastelands.esm" };
}

QString GameFalloutTTW::binaryName() const
{
  return "FalloutNV.exe";
}

QString GameFalloutTTW::gameShortName() const
{
  return "TTW";
}

QStringList GameFalloutTTW::validShortNames() const
{
  return { "FalloutNV", "Fallout3" };
}

QString GameFalloutTTW::gameNexusName() const
{
  return "newvegas";
}

QStringList GameFalloutTTW::iniFiles() const
{
  return { "fallout.ini", "falloutprefs.ini", "falloutcustom.ini",  "custom.ini" };
}

QStringList GameFalloutTTW::DLCPlugins() const
{
  return {};
}

int GameFalloutTTW::nexusModOrganizerID() const
{
  return 42572;
}

int GameFalloutTTW::nexusGameID() const
{
  return 130;
}

QString GameFalloutTTW::getLauncherName() const
{
  return "FalloutNVLauncher.exe";
}

QString GameFalloutTTW::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\FalloutNV";
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(), L"Installed Path");
}
