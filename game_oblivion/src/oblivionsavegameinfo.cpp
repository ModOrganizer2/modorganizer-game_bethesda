#include "oblivionsavegameinfo.h"

#include "oblivionsavegame.h"
#include "gamegamebryo.h"

OblivionSaveGameInfo::OblivionSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

OblivionSaveGameInfo::~OblivionSaveGameInfo()
{
}

MOBase::ISaveGame const *OblivionSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new OblivionSaveGame(file, m_Game);
}

