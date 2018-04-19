#ifndef GAMEFALLOUTTTW_H
#define GAMEFALLOUTTTW_H

#include "gamegamebryo.h"

#include <QObject>
#include <QtGlobal>

class GameFalloutTTW : public GameGamebryo
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.tannin.GameFalloutTTW" FILE "gamefalloutttw.json")
#endif

public:

  GameFalloutTTW();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  virtual QString gameName() const override;
  virtual QList<MOBase::ExecutableInfo> executables() const override;
  virtual void initializeProfile(const QDir &path, ProfileSettings settings) const override;
  virtual QString savegameExtension() const override;
  virtual QString savegameSEExtension() const override;
  virtual QString steamAPPId() const override;
  virtual QStringList primaryPlugins() const override;
  virtual QString binaryName() const override;
  virtual QString gameShortName() const override;
  virtual QStringList validShortNames() const override;
  virtual QString gameNexusName() const override;
  virtual QStringList iniFiles() const override;
  virtual QStringList DLCPlugins() const override;
  virtual SortMechanism sortMechanism() const override;
  virtual int nexusModOrganizerID() const override;
  virtual int nexusGameID() const override;
  virtual QString getLauncherName() const override;

public: // IPlugin interface

  virtual QString name() const override;
  virtual QString author() const override;
  virtual QString description() const override;
  virtual MOBase::VersionInfo version() const override;
  virtual bool isActive() const override;
  virtual QList<MOBase::PluginSetting> settings() const override;

public: // IPluginFileMapper interface

  virtual MappingType mappings() const override;

protected:

  virtual QString identifyGamePath() const override;

};

#endif // GAMEFALLOUTTTW_H
