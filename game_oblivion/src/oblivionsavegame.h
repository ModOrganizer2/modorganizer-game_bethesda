#ifndef OBLIVIONSAVEGAME_H
#define OBLIVIONSAVEGAME_H

#include "gamebryosavegame.h"

class OblivionSaveGame : public GamebryoSaveGame
{
public:
  OblivionSaveGame(QString const &fileName, MOBase::IPluginGame const *game);
};

#endif // OBLIVIONSAVEGAME_H
