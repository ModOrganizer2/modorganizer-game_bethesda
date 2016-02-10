#ifndef SKYRIMSAVEGAME_H
#define SKYRIMSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class SkyrimSaveGame : public GamebryoSaveGame
{
public:
  SkyrimSaveGame(QString const &fileName, MOBase::IPluginGame const *game);
};

#endif // SKYRIMSAVEGAME_H
