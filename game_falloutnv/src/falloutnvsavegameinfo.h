#ifndef FALLOUTNVSAVEGAMEINFO_H
#define FALLOUTNVSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class FalloutNVSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  FalloutNVSaveGameInfo(GameGamebryo const *game);
  ~FalloutNVSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;

};
#endif // FALLOUTNVSAVEGAMEINFO_H
