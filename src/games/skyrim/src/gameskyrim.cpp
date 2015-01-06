#include "gameskyrim.h"
#include <scopeguard.h>
#include <pluginsetting.h>
#include <executableinfo.h>
#include <utility.h>
#include <memory>
#include <QStandardPaths>


using namespace MOBase;


GameSkyrim::GameSkyrim()
{
}

bool GameSkyrim::init(MOBase::IOrganizer *moInfo)
{
  m_GamePath = identifyGamePath();
  m_MyGamesPath = myGamesPath();
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

QString GameSkyrim::identifyGamePath()
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\Bethesda Softworks\\Skyrim", L"Installed Path");
}


QString GameSkyrim::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault) const
{
  PWSTR path = nullptr;
  ON_BLOCK_EXIT([&] () {
    if (path != nullptr) ::CoTaskMemFree(path);
  });

  if (::SHGetKnownFolderPath(folderId, useDefault ? KF_FLAG_DEFAULT_PATH : 0, NULL, &path) == S_OK) {
    return QDir::fromNativeSeparators(QString::fromWCharArray(path));
  } else {
    return QString();
  }
}


QString GameSkyrim::getSpecialPath(const QString &name) const
{
  QString base = findInRegistry(HKEY_CURRENT_USER,
                                L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders",
                                name.toStdWString().c_str());

  WCHAR temp[MAX_PATH];
  if (::ExpandEnvironmentStringsW(base.toStdWString().c_str(), temp, MAX_PATH) != 0) {
    return QString::fromWCharArray(temp);
  } else {
    return base;
  }
}

QString GameSkyrim::myGamesPath()
{
  // a) this is the way it should work. get the configured My Documents directory
  QString result = getKnownFolderPath(FOLDERID_Documents, false);

  // b) if there is no <game> directory there, look in the default directory
  if (result.isEmpty()
      || !QFileInfo(result + "/My Games/Skyrim").exists()) {
    result = getKnownFolderPath(FOLDERID_Documents, true);
  }
  // c) finally, look in the registry. This is discouraged
  if (result.isEmpty()
      || !QFileInfo(result + "/My Games/Skyrim").exists()) {
    result = getSpecialPath("Personal");
  }

  return result + "/My Games/Skyrim";
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
        || !QFileInfo(m_MyGamesPath + "/skyrim.ini").exists()) {
      copyToProfile(m_GamePath, path, "skyrim_default.ini", "skyrim.ini");
    } else {
      copyToProfile(m_MyGamesPath, path, "skyrim.ini");
    }

    copyToProfile(m_MyGamesPath, path, "skyrimprefs.ini");
  }
}

QString GameSkyrim::savegameExtension() const
{
  return "ess";
}

QDir GameSkyrim::savesDirectory() const
{
  return m_MyGamesPath + "/Saves";
}

QDir GameSkyrim::documentsDirectory() const
{
  return m_MyGamesPath;
}

QString GameSkyrim::steamAPPId() const
{
  return "72850";
}
