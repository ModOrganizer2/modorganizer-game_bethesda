#ifndef FALLOUTNVSAVEGAME_H
#define FALLOUTNVSAVEGAME_H

#include "gamebryosavegame.h"

class FalloutNVSaveGame : public GamebryoSaveGame
{
public:
  FalloutNVSaveGame(QString const &fileName);
};

#endif // FALLOUTNVSAVEGAME_H
