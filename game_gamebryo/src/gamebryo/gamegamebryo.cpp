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

#include <fmt/format.h>

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

QStringList GameGamebryo::primarySources() const
{
  return {};
}

QStringList GameGamebryo::validShortNames() const
{
  return {};
}

QStringList GameGamebryo::CCPlugins() const
{
  return {};
}

MOBase::IPluginGame::LoadOrderMechanism GameGamebryo::loadOrderMechanism() const
{
  return LoadOrderMechanism::FileTime;
}

MOBase::IPluginGame::SortMechanism GameGamebryo::sortMechanism() const
{
  return SortMechanism::LOOT;
}

bool GameGamebryo::looksValid(QDir const &path) const
{
  //Check for <prog>.exe for now.
  return path.exists(binaryName());
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

QString GameGamebryo::getProductVersion(QString const& program) const {
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
  if (!::GetFileVersionInfoW(app_name.c_str(), handle, info_len, buff.data())) {
    qDebug("GetFileVersionInfoW Error %d", ::GetLastError());
    return "";
  }

  // The following is from https://stackoverflow.com/a/12408544/2666289

  UINT uiSize;
  BYTE* lpb;
  if (!::VerQueryValueW(buff.data(), TEXT("\\VarFileInfo\\Translation"), (void**)&lpb, &uiSize)) {
    qDebug("VerQueryValue Error %d", ::GetLastError());
    return "";
  }

  WORD* lpw = (WORD*)lpb;
  auto query = fmt::format(L"\\StringFileInfo\\{:04x}{:04x}\\ProductVersion", lpw[0], lpw[1]);
  if (!::VerQueryValueW(buff.data(), query.data(), (void**)&lpb, &uiSize) && uiSize > 0) {
    qDebug("VerQueryValue Error %d", ::GetLastError());
    return "";
  }

  return QString::fromWCharArray((LPCWSTR)lpb);
}

WORD GameGamebryo::getArch(QString const &program) const
{
	WORD arch = 0;
	//This *really* needs to be factored out
	std::wstring app_name =
    L"\\\\?\\" + QDir::toNativeSeparators(this->gameDirectory().absoluteFilePath(program)).toStdWString();

	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = ::FindFirstFileW(app_name.c_str(), &FindFileData);

	//exit if the binary was not found
	if (hFind == INVALID_HANDLE_VALUE) return arch;

  HANDLE hFile = INVALID_HANDLE_VALUE;
  HANDLE hMapping = INVALID_HANDLE_VALUE;
  LPVOID addrHeader = nullptr;
  PIMAGE_NT_HEADERS peHdr = nullptr;

	hFile = CreateFileW(app_name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hFile == INVALID_HANDLE_VALUE) goto cleanup;

	hMapping = CreateFileMappingW(hFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, program.toStdWString().c_str());
	if (hMapping == INVALID_HANDLE_VALUE) goto cleanup;

	addrHeader = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (addrHeader == NULL) goto cleanup; //couldn't memory map the file

	peHdr = ImageNtHeader(addrHeader);
	if (peHdr == NULL) goto cleanup; //couldn't read the header

	arch = peHdr->FileHeader.Machine;

cleanup: //release all of our handles
	FindClose(hFind);
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);
	if (hMapping != INVALID_HANDLE_VALUE)
		CloseHandle(hMapping);
	return arch;
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
                       localAppFolder() + "/" + gameShortName() + "/" + profileFile,
                       false });
  }

  return result;
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY key, LPCWSTR path, LPCWSTR value,
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

QString GameGamebryo::findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value)
{
  std::unique_ptr<BYTE[]> buffer = getRegValue(baseKey, path, value, RRF_RT_REG_SZ | RRF_NOEXPAND);

  return QString::fromUtf16(reinterpret_cast<const ushort*>(buffer.get()));
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault)
{
  PWSTR path = nullptr;
  ON_BLOCK_EXIT([&]() {
    if (path != nullptr) ::CoTaskMemFree(path);
  });

  if (::SHGetKnownFolderPath(folderId, useDefault ? KF_FLAG_DEFAULT_PATH : 0, NULL, &path) == S_OK) {
    return QDir::fromNativeSeparators(QString::fromWCharArray(path));
  }
  else {
    return QString();
  }
}

QString GameGamebryo::getSpecialPath(const QString &name)
{
  QString base = findInRegistry(HKEY_CURRENT_USER,
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders",
    name.toStdWString().c_str());

  WCHAR temp[MAX_PATH];
  if (::ExpandEnvironmentStringsW(base.toStdWString().c_str(), temp, MAX_PATH) != 0) {
    return QString::fromWCharArray(temp);
  }
  else {
    return base;
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
