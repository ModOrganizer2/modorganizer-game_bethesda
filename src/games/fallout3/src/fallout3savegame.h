#ifndef FALLOUT3SAVEGAME_H
#define FALLOUT3SAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class Fallout3SaveGame : public GamebryoSaveGame
{
public:
  Fallout3SaveGame(QString const &fileName, MOBase::IPluginGame const *game);
};

#endif // FALLOUT3SAVEGAME_H
