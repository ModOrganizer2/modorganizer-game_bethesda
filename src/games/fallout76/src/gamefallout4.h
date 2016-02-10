#ifndef GAMEFALLOUT4_H
#define GAMEFALLOUT4_H


#include "gamegamebryo.h"

#include <QObject>
#include <QtGlobal>

class GameFallout4 : public GameGamebryo
{
  Q_OBJECT

  Q_PLUGIN_METADATA(IID "org.tannin.GameFallout4" FILE "gamefallout4.json")

public:

  GameFallout4();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  virtual QString gameName() const override;
  virtual QList<MOBase::ExecutableInfo> executables() const override;
  virtual void initializeProfile(const QDir &path, ProfileSettings settings) const override;
  virtual QString savegameExtension() const override;
  virtual QString steamAPPId() const override;
  virtual QStringList primaryPlugins() const override;
  virtual QStringList gameVariants() const override;
  virtual QString gameShortName() const override;
  virtual QStringList iniFiles() const override;
  virtual QStringList DLCPlugins() const override;
//what load order mechanism?
//  virtual LoadOrderMechanism getLoadOrderMechanism() const = 0;
  virtual int nexusModOrganizerID() const override;
  virtual int nexusGameID() const override;

public: // IPlugin interface

  virtual QString name() const override;
  virtual QString author() const override;
  virtual QString description() const override;
  virtual MOBase::VersionInfo version() const override;
  virtual bool isActive() const override;
  virtual QList<MOBase::PluginSetting> settings() const override;

};

#endif // GAMEFallout4_H
