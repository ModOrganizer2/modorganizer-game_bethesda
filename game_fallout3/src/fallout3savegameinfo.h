#ifndef FALLOUT3SAVEGAMEINFO_H
#define FALLOUT3SAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class Fallout3SaveGameInfo : public GamebryoSaveGameInfo
{
public:
  Fallout3SaveGameInfo(GameGamebryo const *game);
  ~Fallout3SaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // FALLOUT3SAVEGAMEINFO_H
