#ifndef _SKYRIMVRGAMEPLUGINS_H
#define _SKYRIMVRGAMEPLUGINS_H


#include <gamebryogameplugins.h>
#include <iplugingame.h>
#include <imoinfo.h>
#include <map>


class SkyrimVRGamePlugins : public GamebryoGamePlugins
{
public:
  SkyrimVRGamePlugins(MOBase::IOrganizer *organizer);

protected:
  virtual void writePluginList(const MOBase::IPluginList *pluginList,
                               const QString &filePath) override;
  virtual bool readPluginList(MOBase::IPluginList *pluginList,
                              bool useLoadOrder) override;

private:
  std::map<QString, QByteArray> m_LastSaveHash;
};

#endif // _SKYRIMVRGAMEPLUGINS_H
