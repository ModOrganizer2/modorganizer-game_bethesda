#ifndef MORROWINDSAVEGAME_H
#define MORROWINDSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class MorrowindSaveGame : public GamebryoSaveGame
{
public:
  MorrowindSaveGame(QString const &fileName, MOBase::IPluginGame const *game);
};

#endif // MORROWINDSAVEGAME_H
