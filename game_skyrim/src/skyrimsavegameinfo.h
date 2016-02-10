#ifndef SKYRIMSAVEGAMEINFO_H
#define SKYRIMSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class SkyrimSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  SkyrimSaveGameInfo(GameGamebryo const *game);
  ~SkyrimSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // SKYRIMSAVEGAMEINFO_H
