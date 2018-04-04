#ifndef _SKYRIMVRSAVEGAME_H
#define _SKYRIMVRSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class SkyrimVRSaveGame : public GamebryoSaveGame
{
public:
  SkyrimVRSaveGame(QString const &fileName, MOBase::IPluginGame const *game, bool const lightEnabled = true);
};

#endif // _SKYRIMVRSAVEGAME_H
