#ifndef GAMESKYRIM_H
#define GAMESKYRIM_H


#include <iplugingame.h>
#include <QFileInfo>


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

  virtual QList<MOBase::ExecutableInfo> executables();

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

  QString identifyPath();

private:

  QString m_GamePath;
};

#endif // GAMESKYRIM_H
