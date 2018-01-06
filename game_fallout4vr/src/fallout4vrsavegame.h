#ifndef FALLOUT4VRSAVEGAME_H
#define FALLOUT4VRSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class Fallout4VRSaveGame : public GamebryoSaveGame
{
public:
  Fallout4VRSaveGame(QString const &fileName, MOBase::IPluginGame const *game, bool const lightEnabled = true);
};

#endif // FALLOUT4VRSAVEGAME_H
