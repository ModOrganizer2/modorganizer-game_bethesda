#ifndef GAMEBRYOSAVEGAMEINFO_H
#define GAMEBRYOSAVEGAMEINFO_H

#include "savegameinfo.h"

class GameGamebryo;

class GamebryoSaveGameInfo : public SaveGameInfo
{
public:
  GamebryoSaveGameInfo(GameGamebryo const *game);
  ~GamebryoSaveGameInfo();

  virtual MissingAssets getMissingAssets(QString const &file) const override;

protected:
  GameGamebryo const *m_Game;
};

#endif // GAMEBRYOSAVEGAMEINFO_H
