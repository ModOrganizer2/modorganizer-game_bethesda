#ifndef OBLIVIONSAVEGAMEINFO_H
#define OBLIVIONSAVEGAMEINFO_H

#include "savegameinfo.h"

namespace MOBase { class IPluginGame; }

class OblivionSaveGameInfo : public SaveGameInfo
{
public:
  OblivionSaveGameInfo(MOBase::IPluginGame const *game);

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;

private:
  MOBase::IPluginGame const* m_Game;
};
#endif // OBLIVIONSAVEGAMEINFO_H
