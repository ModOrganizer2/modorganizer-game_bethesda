#ifndef FALLOUTNVSAVEGAME_H
#define FALLOUTNVSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class FalloutNVSaveGame : public GamebryoSaveGame
{
public:
  FalloutNVSaveGame(QString const &fileName, MOBase::IPluginGame const *game);
};

#endif // FALLOUTNVSAVEGAME_H
