#ifndef SKYRIMSAVEGAMEINFO_H
#define SKYRIMSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class Fallout4VRSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  Fallout4VRSaveGameInfo(GameGamebryo const *game);
  ~Fallout4VRSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // SKYRIMSAVEGAMEINFO_H
