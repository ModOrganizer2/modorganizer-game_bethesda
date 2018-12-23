#ifndef FALLOUT76SAVEGAME_H
#define FALLOUT76SAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class Fallout76SaveGame : public GamebryoSaveGame
{
public:
  Fallout76SaveGame(QString const &fileName, MOBase::IPluginGame const *game, bool const lightEnabled = true);
};

#endif // FALLOUT76SAVEGAME_H
