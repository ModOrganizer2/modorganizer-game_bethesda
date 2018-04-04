#include "skyrimvrsavegameinfo.h"

#include "skyrimvrsavegame.h"
#include "gamegamebryo.h"

SkyrimVRSaveGameInfo::SkyrimVRSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

SkyrimVRSaveGameInfo::~SkyrimVRSaveGameInfo()
{
}

const MOBase::ISaveGame *SkyrimVRSaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new SkyrimVRSaveGame(file, m_Game);
}
