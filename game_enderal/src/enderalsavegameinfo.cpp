#include "enderalimsavegameinfo.h"

#include "enderalsavegame.h"
#include "gamegamebryo.h"

EnderalSaveGameInfo::EnderalSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

EnderalSaveGameInfo::~EnderalSaveGameInfo()
{
}


MOBase::ISaveGame const *EnderalSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new EnderalSaveGame(file, m_Game);
}
