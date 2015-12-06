#ifndef GAMEFALLOUT4_H
#define GAMEFALLOUT4_H


#include "fallout4scriptextender.h"
#include "gamebryolocalsavegames.h"
#include <gamegamebryo.h>
#include <QFileInfo>


class GameFallout4 : public GameGamebryo
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.tannin.GameFallout4" FILE "gamefallout4.json")

public:

  GameFallout4();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  virtual QString gameName() const;
  virtual QList<MOBase::ExecutableInfo> executables();
  virtual void initializeProfile(const QDir &path, ProfileSettings settings) const;
  virtual QString savegameExtension() const;
  virtual QString steamAPPId() const;
  virtual QStringList getPrimaryPlugins();
  virtual QIcon gameIcon() const override;
  virtual QStringList gameVariants() const;

public: // IPlugin interface

  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;

protected:

  virtual const std::map<std::type_index, boost::any> &featureList() const;

private:

  virtual QString identifyGamePath() const override;
  virtual QString myGamesFolderName() const override;

  void copyToProfile(const QString &sourcePath, const QDir &destinationDirectory,
                     const QString &sourceFileName, const QString &destinationFileName = QString()) const;

private:

  std::shared_ptr<ScriptExtender> m_ScriptExtender { nullptr };
  std::shared_ptr<LocalSavegames> m_LocalSavegames { nullptr };

};

#endif // GAMEFallout4_H
