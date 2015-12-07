#include "gamegamebryo.h"
#include <winreg.h>
#include <utility.h>
#include <scopeguard.h>
#include <QDebug>


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

QDir GameGamebryo::savesDirectory() const
{
  return QDir(m_MyGamesPath + "/Saves");
}

QDir GameGamebryo::documentsDirectory() const
{
  return m_MyGamesPath;
}

bool GameGamebryo::isInstalled() const
{
  return !m_GamePath.isEmpty();
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

QFileInfo GameGamebryo::findInGameFolder(const QString &relativePath)
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

QString GameGamebryo::localAppFolder() const
{
  QString result = getKnownFolderPath(FOLDERID_LocalAppData, false);
  if (result.isEmpty()) {
    // fallback: try the registry
    result = getSpecialPath("Local AppData");
  }

  return result;
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


QStringList GameGamebryo::gameVariants() const
{
  return QStringList();
}


void GameGamebryo::setGameVariant(const QString &variant)
{
  m_GameVariant = variant;
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
