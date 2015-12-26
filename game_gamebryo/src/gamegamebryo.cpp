#include "gamegamebryo.h"

#include "bsainvalidation.h"
#include "dataarchives.h"
#include "savegameinfo.h"
#include "scriptextender.h"
#include "scopeguard.h"
#include "utility.h"

#include <QIcon>

#include <QtDebug>

#include <winreg.h>

#include <vector>

GameGamebryo::GameGamebryo()
{
}

bool GameGamebryo::init(MOBase::IOrganizer *moInfo)
{
  m_GamePath = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath(myGamesFolderName());
  m_Organizer = moInfo;
  return true;
}

bool GameGamebryo::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

QIcon GameGamebryo::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(getBinaryName()));
}

QDir GameGamebryo::gameDirectory() const
{
  return QDir(m_GamePath);
}

QDir GameGamebryo::dataDirectory() const
{
  return gameDirectory().absoluteFilePath("data");
}

void GameGamebryo::setGamePath(const QString &path)
{
  m_GamePath = path;
}

QDir GameGamebryo::documentsDirectory() const
{
  return m_MyGamesPath;
}

QDir GameGamebryo::savesDirectory() const
{
  return QDir(m_MyGamesPath + "/Saves");
}

QStringList GameGamebryo::gameVariants() const
{
  return QStringList();
}

void GameGamebryo::setGameVariant(const QString &variant)
{
  m_GameVariant = variant;
}

QString GameGamebryo::getBinaryName() const
{
  return getGameShortName() + ".exe";
}

MOBase::IPluginGame::LoadOrderMechanism GameGamebryo::getLoadOrderMechanism() const
{
  return LoadOrderMechanism::FileTime;
}

bool GameGamebryo::looksValid(QDir const &path) const
{
  //Check for <prog>.exe and <gamename>Launcher.exe for now.
  return path.exists(getBinaryName()) && path.exists(getLauncherName());
}

QString GameGamebryo::getGameVersion() const
{
  return getVersion(getBinaryName());
}

QString GameGamebryo::getLauncherName() const
{
  return getGameShortName() + "Launcher.exe";
}

QString GameGamebryo::getVersion(const QString &program) const
{
  //This *really* needs to be factored out
  std::wstring app_name = L"\\\\?\\" +
      QDir::toNativeSeparators(this->gameDirectory().absoluteFilePath(program)).toStdWString();
  DWORD handle;
  DWORD info_len = ::GetFileVersionInfoSizeW(app_name.c_str(), &handle);
  if (info_len == 0) {
    qDebug("GetFileVersionInfoSizeW Error %d", ::GetLastError());
    return "";
  }

  std::vector<char> buff(info_len);
  if( ! ::GetFileVersionInfoW(app_name.c_str(), handle, info_len, buff.data())) {
    qDebug("GetFileVersionInfoW Error %d", ::GetLastError());
    return "";
  }

  VS_FIXEDFILEINFO *pFileInfo;
  UINT buf_len;
  if ( ! ::VerQueryValueW(buff.data(), L"\\", reinterpret_cast<LPVOID *>(&pFileInfo), &buf_len)) {
    qDebug("VerQueryValueW Error %d", ::GetLastError());
    return "";
  }
  return QString("%1.%2.%3.%4").arg(HIWORD(pFileInfo->dwFileVersionMS))
                               .arg(LOWORD(pFileInfo->dwFileVersionMS))
                               .arg(HIWORD(pFileInfo->dwFileVersionLS))
                               .arg(LOWORD(pFileInfo->dwFileVersionLS));
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY key, LPCWSTR path,
                                                  LPCWSTR value, DWORD flags,
                                                  LPDWORD type) const
{
  DWORD size = 0;
  HKEY subKey;
  LONG res = ::RegOpenKeyExW(key, path, 0,
                              KEY_QUERY_VALUE | KEY_WOW64_32KEY, &subKey);
  if (res != ERROR_SUCCESS) {
    return std::unique_ptr<BYTE[]>();
  }
  res = ::RegGetValueW(subKey, L"", value, flags, type, nullptr, &size);
  if ((res == ERROR_FILE_NOT_FOUND) || (res == ERROR_UNSUPPORTED_TYPE)) {
    return std::unique_ptr<BYTE[]>();
  } else if ((res != ERROR_SUCCESS) && (res != ERROR_MORE_DATA)) {
    throw MOBase::MyException(QObject::tr("failed to query registry path (preflight): %1").arg(res, 0, 16));
  }

  std::unique_ptr<BYTE[]> result(new BYTE[size]);
  res = ::RegGetValueW(subKey, L"", value, flags, type, result.get(), &size);

  if (res != ERROR_SUCCESS) {
    throw MOBase::MyException(QObject::tr("failed to query registry path (read): %1").arg(res, 0, 16));
  }

  return result;
}

QString GameGamebryo::findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value) const
{
  std::unique_ptr<BYTE[]> buffer = getRegValue(baseKey, path, value, RRF_RT_REG_SZ | RRF_NOEXPAND);

  if (buffer.get() != nullptr) {
    return QString::fromUtf16(reinterpret_cast<const ushort*>(buffer.get()));
  } else {
    return QString();
  }
}

QFileInfo GameGamebryo::findInGameFolder(const QString &relativePath) const
{
  return QFileInfo(m_GamePath + "/" + relativePath);
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault) const
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

QString GameGamebryo::determineMyGamesPath(const QString &gameName)
{
  // a) this is the way it should work. get the configured My Documents directory
  QString result = getKnownFolderPath(FOLDERID_Documents, false);

  // b) if there is no <game> directory there, look in the default directory
  if (result.isEmpty()
      || !QFileInfo(result + "/My Games/" + gameName).exists()) {
    result = getKnownFolderPath(FOLDERID_Documents, true);
  }
  // c) finally, look in the registry. This is discouraged
  if (result.isEmpty()
      || !QFileInfo(result + "/My Games/" + gameName).exists()) {
    result = getSpecialPath("Personal");
  }

  return result + "/My Games/" + gameName;
}

QString GameGamebryo::selectedVariant() const
{
  return m_GameVariant;
}

QString GameGamebryo::getSpecialPath(const QString &name) const
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

QString GameGamebryo::myGamesPath() const
{
  return m_MyGamesPath;
}

QString GameGamebryo::getLootPath() const
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\LOOT", L"Installed Path") + "/Loot.exe";
}

std::map<std::type_index, boost::any> GameGamebryo::featureList() const
{
  static std::map<std::type_index, boost::any> result {
    { typeid(BSAInvalidation), m_BSAInvalidation.get() },
    { typeid(ScriptExtender), m_ScriptExtender.get() },
    { typeid(DataArchives), m_DataArchives.get() },
    { typeid(SaveGameInfo), m_SaveGameInfo.get() }
  };

  return result;
}

QString GameGamebryo::localAppFolder() const
{
  QString result = getKnownFolderPath(FOLDERID_LocalAppData, false);
  if (result.isEmpty()) {
    // fallback: try the registry
    result = getSpecialPath("Local AppData");
  }
  return result;
}

/*
QString GetAppVersion(std::wstring const &app_name)
{
  DWORD handle;
  DWORD info_len = ::GetFileVersionInfoSizeW(app_name.c_str(), &handle);
  if (info_len == 0) {
    qDebug("GetFileVersionInfoSizeW Error %d", ::GetLastError());
    return "";
  }

  std::vector<char> buff(info_len);
  if( ! ::GetFileVersionInfoW(app_name.c_str(), handle, info_len, buff.data())) {
    qDebug("GetFileVersionInfoW Error %d", ::GetLastError());
    return "";
  }

  VS_FIXEDFILEINFO *pFileInfo;
  UINT buf_len;
  if ( ! ::VerQueryValueW(buff.data(), L"\\", reinterpret_cast<LPVOID *>(&pFileInfo), &buf_len)) {
    qDebug("VerQueryValueW Error %d", ::GetLastError());
    return "";
  }
  return QString("%1.%2.%3.%4").arg(HIWORD(pFileInfo->dwFileVersionMS))
                               .arg(LOWORD(pFileInfo->dwFileVersionMS))
                               .arg(HIWORD(pFileInfo->dwFileVersionLS))
                               .arg(LOWORD(pFileInfo->dwFileVersionLS));
}
*/
