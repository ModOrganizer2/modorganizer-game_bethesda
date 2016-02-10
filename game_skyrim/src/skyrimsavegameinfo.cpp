#include "skyrimsavegameinfo.h"

#include "skyrimsavegame.h"
#include "gamegamebryo.h"

SkyrimSaveGameInfo::SkyrimSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

SkyrimSaveGameInfo::~SkyrimSaveGameInfo()
{
}


MOBase::ISaveGame const *SkyrimSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new SkyrimSaveGame(file, m_Game);
}
