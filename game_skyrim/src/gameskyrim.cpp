#include "gameskyrim.h"
#include <pluginsetting.h>
#include <executableinfo.h>
#include <utility.h>
#include <memory>


using namespace MOBase;


GameSkyrim::GameSkyrim()
{
}

bool GameSkyrim::init(MOBase::IOrganizer *moInfo)
{
  m_GamePath = identifyPath();
  qDebug("found: %s", qPrintable(m_GamePath));
  return true;
}

std::unique_ptr<BYTE[]> getRegValue(HKEY key, LPCWSTR subKey, LPCWSTR value, DWORD flags, LPDWORD type = nullptr)
{
  DWORD size = 0;
  DWORD res = ::RegGetValueW(key, subKey, value, flags, type, nullptr, &size);
  if (res == ERROR_FILE_NOT_FOUND) {
    return std::unique_ptr<BYTE[]>();
  } else if ((res != ERROR_SUCCESS) && (res != ERROR_MORE_DATA)) {
    throw MyException(QObject::tr("failed to query registry path (preflight): %1").arg(res, 0, 16));
  }

  std::unique_ptr<BYTE[]> result(new BYTE[size]);
  res = ::RegGetValueW(key, subKey, value, flags, type, result.get(), &size);

  if (res != ERROR_SUCCESS) {
    throw MyException(QObject::tr("failed to query registry path (read): %1").arg(res, 0, 16));
  }

  return result;
}

QString findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value)
{
  std::unique_ptr<BYTE[]> buffer = getRegValue(baseKey, path, value, RRF_RT_REG_SZ | RRF_NOEXPAND);

  if (buffer.get() != nullptr) {
    return QString::fromUtf16(reinterpret_cast<const ushort*>(buffer.get()));
  } else {
    return QString();
  }
}

QString GameSkyrim::identifyPath()
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\Bethesda Softworks\\Skyrim", L"Installed Path");
}

QString GameSkyrim::gameName() const
{
  return "Skyrim";
}

QDir GameSkyrim::gameDirectory() const
{
  return QDir(m_GamePath);
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
  return tr("Adds support for the game Sykrim");
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

QFileInfo GameSkyrim::findInGameFolder(const QString &relativePath)
{
  return QFileInfo(m_GamePath + "/" + relativePath);
}
