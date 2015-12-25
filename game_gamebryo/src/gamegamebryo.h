#ifndef GAMEGAMEBRYO_H
#define GAMEGAMEBRYO_H

#include "iplugingame.h"

class ScriptExtender;
class DataArchives;
class SaveGameInfo;
class BSAInvalidation;

#include <QString>

#include <memory>

#include <ShlObj.h>


class GameGamebryo : public MOBase::IPluginGame
{
  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginGame)

  friend class GamebryoScriptExtender;
  friend class GamebryoSaveGameInfo;
  friend class GamebryoSaveGameInfoWidget;

public:

  GameGamebryo();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  //getName
  //initializeProfile
  //savegameExtension
  virtual bool isInstalled() const override;
  virtual QIcon gameIcon() const override;
  virtual QDir gameDirectory() const override;
  virtual QDir dataDirectory() const override;
  virtual void setGamePath(const QString &path) override;
  virtual QDir documentsDirectory() const override;
  virtual QDir savesDirectory() const override;
  //executables
  //steamAPPId
  //getPrimaryPlugins
  virtual QStringList gameVariants() const override;
  virtual void setGameVariant(const QString &variant) override;
  virtual QString getBinaryName() const override;
  //getGameShortName
  //getIniFiles
  //getDLCPlugins
  virtual LoadOrderMechanism getLoadOrderMechanism() const override;
  //getNexusModOrganizerID
  //getNexusGameID
  virtual bool looksValid(QDir const &) const override;
  virtual QString getGameVersion() const override;

protected:

  std::unique_ptr<BYTE[]> getRegValue(HKEY key, LPCWSTR subKey, LPCWSTR value, DWORD flags, LPDWORD type = nullptr) const;
  QString findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value) const;
  QFileInfo findInGameFolder(const QString &relativePath) const;
  QString getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault) const;
  QString getSpecialPath(const QString &name) const;
  QString myGamesPath() const;
  //Arguably this shouldn't really be here but every gamebryo program seems to use it
  QString getLootPath() const;
  QString selectedVariant() const;
  virtual QString getLauncherName() const;
  QString getVersion(QString const &program) const;

protected:

  std::map<std::type_index, boost::any> featureList() const;

  //These should be implemented by anything that uses gamebro (I think)
  //(and if they don't, it'll be a null pointer and won't look implemented,
  //so that's fine too).
  std::shared_ptr<ScriptExtender> m_ScriptExtender { nullptr };
  std::shared_ptr<DataArchives> m_DataArchives { nullptr };
  std::shared_ptr<BSAInvalidation> m_BSAInvalidation { nullptr };
  std::shared_ptr<SaveGameInfo> m_SaveGameInfo { nullptr };

private:

  QString determineMyGamesPath(const QString &gameName);

  virtual QString myGamesFolderName() const = 0;
  virtual QString identifyGamePath() const = 0;

private:

  QString m_GamePath;
  QString m_MyGamesPath;

  QString m_GameVariant;

  MOBase::IOrganizer *m_Organizer;

};

#endif // GAMEGAMEBRYO_H
