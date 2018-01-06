#include "fallout4vrsavegameinfo.h"

#include "fallout4vrsavegame.h"
#include "gamegamebryo.h"

Fallout4VRSaveGameInfo::Fallout4VRSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

Fallout4VRSaveGameInfo::~Fallout4VRSaveGameInfo()
{
}

const MOBase::ISaveGame *Fallout4VRSaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new Fallout4VRSaveGame(file, m_Game);
}

