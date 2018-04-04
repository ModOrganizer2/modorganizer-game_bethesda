#ifndef _SKYRIMVRSAVEGAMEINFO_H
#define _SKYRIMVRSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class SkyrimVRSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  SkyrimVRSaveGameInfo(GameGamebryo const *game);
  ~SkyrimVRSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // _SKYRIMVRSAVEGAMEINFO_H
