#ifndef FALLOUTTTWSAVEGAMEINFO_H
#define FALLOUTTTWSAVEGAMEINFO_H

#include "gamebryosavegameinfo.h"

class GameGamebryo;

class FalloutTTWSaveGameInfo : public GamebryoSaveGameInfo
{
public:
  FalloutTTWSaveGameInfo(GameGamebryo const *game);
  ~FalloutTTWSaveGameInfo();

  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;

};
#endif // FALLOUTTTWSAVEGAMEINFO_H
