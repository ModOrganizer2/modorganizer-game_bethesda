#ifndef _GAMESKYRIMVR_H
#define _GAMESKYRIMVR_H

#include "gamegamebryo.h"

#include <QObject>
#include <QtGlobal>

class GameSkyrimVR : public GameGamebryo
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.soundcontactstudio.GameSkyrimVR" FILE "gameskyrimVR.json")

public:
    GameSkyrimVR();

    virtual bool init(MOBase::IOrganizer *moInfo) override;

public: // IPluginGame interface
    virtual QString gameName() const override;

    virtual QList<MOBase::ExecutableInfo> executables() const override;
    virtual void initializeProfile(const QDir &path, ProfileSettings settings) const override;
    virtual QString savegameExtension() const override;
    virtual QString savegameSEExtension() const override;
    virtual QString steamAPPId() const override;
    virtual QStringList primaryPlugins() const override;
    virtual QStringList gameVariants() const override;
    virtual QString gameShortName() const override;
    virtual QString gameNexusName() const override;
    virtual QStringList iniFiles() const override;
    virtual QStringList DLCPlugins() const override;
    virtual QStringList CCPlugins() const override;
    virtual LoadOrderMechanism loadOrderMechanism() const override;
    virtual int nexusModOrganizerID() const override;
    virtual int nexusGameID() const override;
    virtual QString getLauncherName() const override;
	virtual QString GameSkyrimVR::binaryName() const;

    virtual bool isInstalled() const override;
    virtual void setGamePath(const QString &path) override;
    virtual QDir gameDirectory() const override;

public: // IPlugin interface
    virtual QString name() const override;
    virtual QString author() const override;
    virtual QString description() const override;
    virtual MOBase::VersionInfo version() const override;
    virtual bool isActive() const override;
    virtual QList<MOBase::PluginSetting> settings() const override;

public: // IPluginFileMapper
    virtual MappingType mappings() const;

protected:
    QDir documentsDirectory() const;
    QDir savesDirectory() const;
    QFileInfo findInGameFolder(const QString &relativePath) const;
    QString myGamesPath() const;

private:
    MOBase::IOrganizer *m_Organizer;
    QString identifyGamePath() const;
    QString m_GamePath;
    QString m_MyGamesPath;
};

#endif // _GAMESKYRIMVR_H
