#ifndef _SKYRIMGAMEPLUGINS_H
#define _SKYRIMGAMEPLUGINS_H


#include <gamebryogameplugins.h>
#include <iplugingame.h>
#include <imoinfo.h>
#include <map>


class SkyrimGamePlugins : public GamebryoGamePlugins
{
public:
    SkyrimGamePlugins(MOBase::IOrganizer *organizer);

    virtual void readPluginLists(MOBase::IPluginList *pluginList) override;

protected:
    virtual bool readPluginList(MOBase::IPluginList *pluginList,
        bool useLoadOrder) override;

private:
    std::map<QString, QByteArray> m_LastSaveHash;

private:
    QTextCodec *m_LocalCodec;
};

#endif // _SKYRIMSEGAMEPLUGINS_H
