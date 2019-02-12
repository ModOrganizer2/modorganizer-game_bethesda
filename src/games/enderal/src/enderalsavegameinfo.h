#ifndef ENDERALSAVEGAMEINFO_H
#define ENDERALSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class EnderalSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  EnderalSaveGameInfo(GameGamebryo const *game);
  ~EnderalSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // ENDERALSAVEGAMEINFO_H
