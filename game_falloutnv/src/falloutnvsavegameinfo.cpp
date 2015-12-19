#include "falloutnvsavegameinfo.h"

#include "falloutnvsavegame.h"
#include "gamegamebryo.h"

FalloutNVSaveGameInfo::FalloutNVSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

FalloutNVSaveGameInfo::~FalloutNVSaveGameInfo()
{
}

MOBase::ISaveGame const *FalloutNVSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new FalloutNVSaveGame(file, m_Game);
}
