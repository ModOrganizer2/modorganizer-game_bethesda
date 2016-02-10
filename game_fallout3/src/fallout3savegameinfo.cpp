#include "fallout3savegameinfo.h"

#include "fallout3savegame.h"
#include "gamegamebryo.h"

Fallout3SaveGameInfo::Fallout3SaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

Fallout3SaveGameInfo::~Fallout3SaveGameInfo()
{
}

MOBase::ISaveGame const *Fallout3SaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new Fallout3SaveGame(file, m_Game);
}

