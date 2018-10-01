#ifndef CREATIONGAMEPLUGINS_H
#define CREATIONGAMEPLUGINS_H

#include <gamebryogameplugins.h>
#include <iplugingame.h>
#include <imoinfo.h>
#include <map>

class CreationGamePlugins : public GamebryoGamePlugins
{
public:
  CreationGamePlugins(MOBase::IOrganizer *organizer);

protected:
  virtual void writePluginList(const MOBase::IPluginList *pluginList,
                               const QString &filePath) override;
  virtual QStringList readPluginList(MOBase::IPluginList *pluginList) override;
  virtual void getLoadOrder(QStringList &loadOrder) override;

private:
  std::map<QString, QByteArray> m_LastSaveHash;
};

#endif // CREATIONGAMEPLUGINS_H