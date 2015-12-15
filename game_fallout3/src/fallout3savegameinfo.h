#ifndef FALLOUT3SAVEGAMEINFO_H
#define FALLOUT3SAVEGAMEINFO_H

#include "savegameinfo.h"

namespace MOBase { class IPluginGame; }

class Fallout3SaveGameInfo : public SaveGameInfo
{
public:
  Fallout3SaveGameInfo(MOBase::IPluginGame const *game);
  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
private:
  MOBase::IPluginGame const* m_Game;
};

#endif // FALLOUT3SAVEGAMEINFO_H
