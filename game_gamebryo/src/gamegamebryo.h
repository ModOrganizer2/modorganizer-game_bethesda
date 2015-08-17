#ifndef GAMEGAMEBRYO_H
#define GAMEGAMEBRYO_H


#include <iplugingame.h>
#include <memory>
#include <ShlObj.h>


class GameGamebryo : public MOBase::IPluginGame
{
  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginGame)

public:

  GameGamebryo();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  virtual QDir gameDirectory() const;
  virtual QDir dataDirectory() const;
  virtual void setGamePath(const QString &path);
  virtual QDir savesDirectory() const;
  virtual QDir documentsDirectory() const;

  virtual bool isInstalled() const override;

  virtual QStringList gameVariants() const;
  virtual void setGameVariant(const QString &variant);

protected:

  std::unique_ptr<BYTE[]> getRegValue(HKEY key, LPCWSTR subKey, LPCWSTR value, DWORD flags, LPDWORD type = nullptr) const;
  QString findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value) const;
  QFileInfo findInGameFolder(const QString &relativePath);
  QString getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault) const;
  QString getSpecialPath(const QString &name) const;
  QString myGamesPath() const;
  //Arguably this shouldn't really be here but every gamebryo program seems to use it
  QString getLootPath() const;
  QString selectedVariant() const;

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
