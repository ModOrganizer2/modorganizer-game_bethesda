#ifndef GAMESKYRIM_H
#define GAMESKYRIM_H


#include <iplugingame.h>
#include <QFileInfo>
#include <ShlObj.h>


class GameSkyrim : public MOBase::IPluginGame
{
  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginGame)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.tannin.DiagnoseBasic" FILE "gameskyrim.json")
#endif

public:
  GameSkyrim();

  // IPluginGame interface
public:

  virtual QString gameName() const;
  virtual QDir gameDirectory() const;
  virtual QDir savesDirectory() const;
  virtual QDir documentsDirectory() const;
  virtual QList<MOBase::ExecutableInfo> executables();
  virtual void initializeProfile(const QDir &path, ProfileSettings settings) const;
  virtual QString savegameExtension() const;
  virtual QString steamAPPId() const;

  // IPlugin interface
public:
  virtual bool init(MOBase::IOrganizer *moInfo);
  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;

private:

  QFileInfo findInGameFolder(const QString &relativePath);

  QString identifyGamePath();
  QString myGamesPath();
  QString getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault) const;
  QString getSpecialPath(const QString &name) const;
  QString localAppFolder() const;
  void copyToProfile(const QString &sourcePath, const QDir &destinationDirectory,
                     const QString &sourceFileName, const QString &destinationFileName = QString()) const;

private:

  QString m_GamePath;
  QString m_MyGamesPath;

};

#endif // GAMESKYRIM_H
