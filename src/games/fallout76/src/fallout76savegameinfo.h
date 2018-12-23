#ifndef FALLOUT76SAVEGAMEINFO_H
#define FALLOUT76SAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class Fallout76SaveGameInfo : public GamebryoSaveGameInfo
{
public:
  Fallout76SaveGameInfo(GameGamebryo const *game);
  ~Fallout76SaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // FALLOUT76SAVEGAMEINFO_H
