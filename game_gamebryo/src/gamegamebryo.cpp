#include "gamegamebryo.h"

#include "bsainvalidation.h"
#include "dataarchives.h"
#include "savegameinfo.h"
#include "scriptextender.h"
#include "scopeguard.h"
#include "utility.h"

#include <QDir>
#include <QIcon>
#include <QFile>
#include <QFileInfo>

#include <QtDebug>
#include <QtGlobal>

#include <Knownfolders.h>
#include <Shlobj.h>
#include <Windows.h>
#include <winreg.h>
#include <winver.h>

#include <string>
#include <stddef.h>
#include <vector>

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

QString getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault)
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

QString getSpecialPath(const QString &name)
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

QString determineMyGamesPath(const QString &gameName)
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

}

GameGamebryo::GameGamebryo()
{
}

bool GameGamebryo::init(MOBase::IOrganizer *moInfo)
{
  m_GamePath = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath(gameShortName());
  m_Organizer = moInfo;
  return true;
}

bool GameGamebryo::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

QIcon GameGamebryo::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(binaryName()));
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

QString GameGamebryo::binaryName() const
{
  return gameShortName() + ".exe";
}

MOBase::IPluginGame::LoadOrderMechanism GameGamebryo::loadOrderMechanism() const
{
  return LoadOrderMechanism::FileTime;
}

bool GameGamebryo::looksValid(QDir const &path) const
{
  //Check for <prog>.exe and <gamename>Launcher.exe for now.
  return path.exists(binaryName()) && path.exists(getLauncherName());
}

QString GameGamebryo::gameVersion() const
{
  return getVersion(binaryName());
}

QString GameGamebryo::getLauncherName() const
{
  return gameShortName() + "Launcher.exe";
}

QString GameGamebryo::getVersion(QString const &program) const
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

QFileInfo GameGamebryo::findInGameFolder(const QString &relativePath) const
{
  return QFileInfo(m_GamePath + "/" + relativePath);
}

QString GameGamebryo::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\" + gameShortName();
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(), L"Installed Path");
}

QString GameGamebryo::selectedVariant() const
{
  return m_GameVariant;
}

QString GameGamebryo::myGamesPath() const
{
  return m_MyGamesPath;
}

/*static*/ QString GameGamebryo::getLootPath()
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\LOOT", L"Installed Path") + "/Loot.exe";
}

std::map<std::type_index, boost::any> GameGamebryo::featureList() const
{
  return m_FeatureList;
}

QString GameGamebryo::localAppFolder()
{
  QString result = getKnownFolderPath(FOLDERID_LocalAppData, false);
  if (result.isEmpty()) {
    // fallback: try the registry
    result = getSpecialPath("Local AppData");
  }
  return result;
}

void GameGamebryo::copyToProfile(QString const &sourcePath,
                                 QDir const &destinationDirectory,
                                 QString const &sourceFileName) {
  copyToProfile(sourcePath, destinationDirectory, sourceFileName,
                sourceFileName);
}

void GameGamebryo::copyToProfile(QString const &sourcePath,
                                 QDir const &destinationDirectory,
                                 QString const &sourceFileName,
                                 QString const &destinationFileName) {
  QString filePath = destinationDirectory.absoluteFilePath(destinationFileName);
  if (!QFileInfo(filePath).exists()) {
    if (!MOBase::shellCopy(sourcePath + "/" + sourceFileName, filePath)) {
      // if copy file fails, create the file empty
      QFile(filePath).open(QIODevice::WriteOnly);
    }
  }
}

MappingType GameGamebryo::mappings() const
{
  MappingType result;

  for (const QString &profileFile : { "plugins.txt", "loadorder.txt" }) {
    result.push_back({ m_Organizer->profilePath() + "/" + profileFile,
                       localAppFolder() + "/" + gameName().replace(" ", "") + "/" + profileFile,
                       false });
  }

  return result;
}
