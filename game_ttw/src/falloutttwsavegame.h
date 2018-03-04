#ifndef FALLOUTTTWSAVEGAME_H
#define FALLOUTTTWSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class FalloutTTWSaveGame : public GamebryoSaveGame
{
public:
  FalloutTTWSaveGame(QString const &fileName, MOBase::IPluginGame const *game);
};

#endif // FALLOUTTTWSAVEGAME_H
