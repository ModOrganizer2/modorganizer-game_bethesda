#ifndef FALLOUT4VRGAMEPLUGINS_H
#define FALLOUT4VRGAMEPLUGINS_H


#include <gamebryogameplugins.h>
#include <iplugingame.h>
#include <imoinfo.h>
#include <map>


class Fallout4VRGamePlugins : public GamebryoGamePlugins
{
public:
  Fallout4VRGamePlugins(MOBase::IOrganizer *organizer);

protected:
  virtual void writePluginList(const MOBase::IPluginList *pluginList,
                               const QString &filePath) override;
  virtual bool readPluginList(MOBase::IPluginList *pluginList,
                              bool useLoadOrder) override;

private:
  std::map<QString, QByteArray> m_LastSaveHash;
};

#endif // FALLOUT4VRGAMEPLUGINS_H
