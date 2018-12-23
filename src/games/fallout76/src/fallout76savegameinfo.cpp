#include "fallout76savegameinfo.h"

#include "fallout76savegame.h"
#include "gamegamebryo.h"

Fallout76SaveGameInfo::Fallout76SaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

Fallout76SaveGameInfo::~Fallout76SaveGameInfo()
{
}

const MOBase::ISaveGame *Fallout76SaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new Fallout76SaveGame(file, m_Game);
}

