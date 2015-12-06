#ifndef SKYRIMSAVEGAME_H
#define SKYRIMSAVEGAME_H

#include "gamebryosavegame.h"

class SkyrimSaveGame : public GamebryoSaveGame
{
public:
  SkyrimSaveGame(QString const &fileName);
};

#endif // SKYRIMSAVEGAME_H
