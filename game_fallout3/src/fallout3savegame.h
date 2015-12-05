#ifndef FALLOUT3SAVEGAME_H
#define FALLOUT3SAVEGAME_H

#include "gamebryosavegame.h"

class Fallout3SaveGame : public GamebryoSaveGame
{
public:
  Fallout3SaveGame(QString const &fileName);
};

#endif // FALLOUT3SAVEGAME_H
