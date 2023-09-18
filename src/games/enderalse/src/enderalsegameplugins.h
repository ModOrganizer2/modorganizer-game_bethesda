#ifndef ENDERALSEGAMEPLUGINS_H
#define ENDERALSEGAMEPLUGINS_H

#include <creationgameplugins.h>
#include <imoinfo.h>
#include <iplugingame.h>
#include <map>

class EnderalSEGamePlugins : public CreationGamePlugins
{
public:
  using CreationGamePlugins::CreationGamePlugins;

protected:
  void writePluginList(const MOBase::IPluginList* pluginList,
                       const QString& filePath) override;

private:
  std::map<QString, QByteArray> m_LastSaveHash;
};

#endif  // ENDERALSEGAMEPLUGINS_H