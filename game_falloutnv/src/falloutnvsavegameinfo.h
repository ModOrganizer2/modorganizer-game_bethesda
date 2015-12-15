#ifndef FALLOUTNVSAVEGAMEINFO_H
#define FALLOUTNVSAVEGAMEINFO_H

#include "savegameinfo.h"

namespace MOBase { class IPluginGame; }

class FalloutNVSaveGameInfo : public SaveGameInfo
{
public:
  FalloutNVSaveGameInfo(MOBase::IPluginGame const *game);
  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;

private:
  MOBase::IPluginGame const* m_Game;

};
#endif // FALLOUTNVSAVEGAMEINFO_H
