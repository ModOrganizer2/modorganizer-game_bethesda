#ifndef OBLIVIONSAVEGAMEINFO_H
#define OBLIVIONSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class OblivionSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  OblivionSaveGameInfo(GameGamebryo const *game);
  ~OblivionSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;

};
#endif // OBLIVIONSAVEGAMEINFO_H
