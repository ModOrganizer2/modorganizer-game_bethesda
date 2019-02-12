#ifndef ENDERALSAVEGAME_H
#define ENDERALSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class EnderalSaveGame : public GamebryoSaveGame
{
public:
  EnderalSaveGame(QString const &fileName, MOBase::IPluginGame const *game);
};

#endif // ENDERALSAVEGAME_H
