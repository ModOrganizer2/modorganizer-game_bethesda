#include "gameskyrimvr.h"

#include "skyrimvrdataarchives.h"
#include "skyrimvrscriptextender.h"
#include "skyrimvrsavegameinfo.h"
#include "skyrimvrgameplugins.h"
#include "skyrimvrunmanagedmods.h"

#include <pluginsetting.h>
#include "iplugingame.h"
#include <executableinfo.h>
#include <gamebryolocalsavegames.h>
#include <gamebryogameplugins.h>
#include "versioninfo.h"

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

    QString getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault)
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


    QString getSpecialPath(const QString &name)
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


using namespace MOBase;

GameSkyrimVR::GameSkyrimVR()
{
}

void GameSkyrimVR::setGamePath(const QString &path)
{
    m_GamePath = path;
}

QDir GameSkyrimVR::documentsDirectory() const
{
    return m_MyGamesPath;
}

QString GameSkyrimVR::identifyGamePath() const
{
    QString path = "Software\\Bethesda Softworks\\" + gameName();
    return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(), L"Installed Path");
}

QDir GameSkyrimVR::savesDirectory() const
{
    return QDir(m_MyGamesPath + "/Saves");
}

QString GameSkyrimVR::myGamesPath() const
{
    return m_MyGamesPath;
}

bool GameSkyrimVR::isInstalled() const
{
    return !m_GamePath.isEmpty();
}

bool GameSkyrimVR::init(IOrganizer *moInfo)
{
    if (!GameGamebryo::init(moInfo)) {
        return false;
    }

    m_Organizer = moInfo;
    m_GamePath = GameSkyrimVR::identifyGamePath();
    m_MyGamesPath = determineMyGamesPath(gameName());


    registerFeature<ScriptExtender>(new SkyrimVRScriptExtender(this));
    registerFeature<DataArchives>(new SkyrimVRDataArchives(myGamesPath()));
    registerFeature<LocalSavegames>(new GamebryoLocalSavegames(myGamesPath(), "SkyrimVR.ini"));
    registerFeature<SaveGameInfo>(new SkyrimVRSaveGameInfo(this));
    registerFeature<GamePlugins>(new SkyrimVRGamePlugins(moInfo));
    registerFeature<UnmanagedMods>(new SkyrimVRUnmangedMods(this));

    return true;
}



QString GameSkyrimVR::gameName() const
{
    return "Skyrim VR";
}

QList<ExecutableInfo> GameSkyrimVR::executables() const
{
  return QList<ExecutableInfo>()
    << ExecutableInfo("SKSE", findInGameFolder(feature<ScriptExtender>()->loaderName()))
    << ExecutableInfo("Skyrim VR", findInGameFolder(binaryName()))
    << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
    << ExecutableInfo("LOOT", getLootPath()).withArgument("--game=\"Skyrim Special Edition\"")
    ;
}

QFileInfo GameSkyrimVR::findInGameFolder(const QString &relativePath) const
{
    return QFileInfo(m_GamePath + "/" + relativePath);
}

QString GameSkyrimVR::name() const
{
    return "Skyrim VR Support Plugin";
}

QString GameSkyrimVR::author() const
{
    return "Brixified";
}

QString GameSkyrimVR::description() const
{
    return tr("Adds support for the game Skyrim VR.");
}

MOBase::VersionInfo GameSkyrimVR::version() const
{
    return VersionInfo(0, 1, 5, VersionInfo::RELEASE_ALPHA);
}

bool GameSkyrimVR::isActive() const
{
    return qApp->property("managed_game").value<IPluginGame*>() == this;
}

QList<PluginSetting> GameSkyrimVR::settings() const
{
    return QList<PluginSetting>();
}

void GameSkyrimVR::initializeProfile(const QDir &path, ProfileSettings settings) const
{
    if (settings.testFlag(IPluginGame::MODS)) {
        copyToProfile(localAppFolder() + "/Skyrim VR", path, "plugins.txt");
        copyToProfile(localAppFolder() + "/Skyrim VR", path, "loadorder.txt");
    }

    if (settings.testFlag(IPluginGame::CONFIGURATION)) {
      if (settings.testFlag(IPluginGame::PREFER_DEFAULTS)
        || !QFileInfo(myGamesPath() + "/skyrimvr.ini").exists()) {
        copyToProfile(gameDirectory().absolutePath(), path, "skyrim.ini", "skyrimvr.ini");
      } else {
        copyToProfile(myGamesPath(), path, "skyrimvr.ini");
      }

      copyToProfile(myGamesPath(), path, "skyrimprefs.ini");
    }
}

QString GameSkyrimVR::savegameExtension() const
{
    return "ess";
}

QString GameSkyrimVR::savegameSEExtension() const
{
    return "skse";
}

QString GameSkyrimVR::steamAPPId() const
{
    return "611670";
}

QStringList GameSkyrimVR::primaryPlugins() const {
  QStringList plugins = { "skyrim.esm", "update.esm", "dawnguard.esm", "hearthfires.esm", "dragonborn.esm", "skyrimvr.esm" };

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameSkyrimVR::gameVariants() const
{
    return{ "Regular" };
}

QString GameSkyrimVR::gameShortName() const
{
    return "SkyrimVR";
}

QStringList GameSkyrimVR::validShortNames() const
{
    return { "Skyrim", "SkyrimSE" };
}

QString GameSkyrimVR::gameNexusName() const
{
    return "skyrimspecialedition";
}


QStringList GameSkyrimVR::iniFiles() const
{
  return{ "skyrimvr.ini", "skyrimprefs.ini" };
}

QStringList GameSkyrimVR::DLCPlugins() const
{
    return{ "dawnguard.esm", "hearthfires.esm", "dragonborn.esm" };
}

QStringList GameSkyrimVR::CCPlugins() const
{
  QStringList plugins = {};
  QFile file(gameDirectory().filePath("Skyrim.ccc"));
  if (file.open(QIODevice::ReadOnly)) {
    ON_BLOCK_EXIT([&file]() { file.close(); });

    if (file.size() == 0) {
      return plugins;
    }
    while (!file.atEnd()) {
      QByteArray line = file.readLine().trimmed();
      QString modName;
      if ((line.size() > 0) && (line.at(0) != '#')) {
        modName = QString::fromUtf8(line.constData()).toLower();
      }

      if (modName.size() > 0) {
        if (!plugins.contains(modName, Qt::CaseInsensitive)) {
          plugins.append(modName);
        }
      }
    }
  }
  return plugins;
}

IPluginGame::LoadOrderMechanism GameSkyrimVR::loadOrderMechanism() const
{
    return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameSkyrimVR::nexusModOrganizerID() const
{
    return 6194; //... Should be 0?
}

int GameSkyrimVR::nexusGameID() const
{
    return 1704; //1704
}

QString GameSkyrimVR::getLauncherName() const
{
    return binaryName(); // Skyrim VR has no Launcher, so we just return the name of the game binary
}

QDir GameSkyrimVR::gameDirectory() const
{
    return QDir(m_GamePath);
}

QString GameSkyrimVR::binaryName() const
{
    return "SkyrimVR.exe";
}

// Not to delete all the spaces...
MappingType GameSkyrimVR::mappings() const
{
    MappingType result;

    for (const QString &profileFile : { "plugins.txt", "loadorder.txt" }) {
        result.push_back({ m_Organizer->profilePath() + "/" + profileFile,
            localAppFolder() + "/" + gameName() + "/" + profileFile,
            false });
    }

    return result;
}

