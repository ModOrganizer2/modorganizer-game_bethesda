#ifndef GAMEFALLOUTNV_H
#define GAMEFALLOUTNV_H


#include "falloutnvbsainvalidation.h"
#include "falloutnvscriptextender.h"
#include "falloutnvdataarchives.h"
#include <gamegamebryo.h>
#include <QFileInfo>


class GameFalloutNV : public GameGamebryo
{
  Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.tannin.GameFalloutNV" FILE "gamefalloutnv.json")
#endif

public:

  GameFalloutNV();

  virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface

  virtual QString gameName() const override;
  virtual QList<MOBase::ExecutableInfo> executables() const override;
  virtual void initializeProfile(const QDir &path, ProfileSettings settings) const override;
  virtual QString savegameExtension() const override;
  virtual QString steamAPPId() const override;
  virtual QStringList getPrimaryPlugins() const override;
  virtual QString getGameShortName() const override;
  virtual QStringList getIniFiles() const override;
  virtual QStringList getDLCPlugins() const override;
  virtual int getNexusModOrganizerID() const override;
  virtual int getNexusGameID() const override;

public: // IPlugin interface

  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;

protected:

  virtual std::map<std::type_index, boost::any> featureList() const override;

private:

  virtual QString identifyGamePath() const override;
  virtual QString myGamesFolderName() const override;

  QString localAppFolder() const;
  void copyToProfile(const QString &sourcePath, const QDir &destinationDirectory,
                     const QString &sourceFileName, const QString &destinationFileName = QString()) const;

private:

  std::shared_ptr<ScriptExtender> m_ScriptExtender { nullptr };
  std::shared_ptr<DataArchives> m_DataArchives { nullptr };
  std::shared_ptr<BSAInvalidation> m_BSAInvalidation { nullptr };

};

#endif // GAMEFALLOUTNV_H
