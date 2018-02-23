#ifndef MORROWINDSAVEGAMEINFO_H
#define MORROWINDSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class MorrowindSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  MorrowindSaveGameInfo(GameGamebryo const *game);
  ~MorrowindSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // MORROWINDSAVEGAMEINFO_H
