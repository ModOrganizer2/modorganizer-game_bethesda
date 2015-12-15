#ifndef SKYRIMSAVEGAMEINFO_H
#define SKYRIMSAVEGAMEINFO_H

#include "savegameinfo.h"

namespace MOBase { class IPluginGame; }

class SkyrimSaveGameInfo : public SaveGameInfo
{
public:
  SkyrimSaveGameInfo(MOBase::IPluginGame const *game);
  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
private:
  MOBase::IPluginGame const *m_Game;

};

#endif // SKYRIMSAVEGAMEINFO_H
