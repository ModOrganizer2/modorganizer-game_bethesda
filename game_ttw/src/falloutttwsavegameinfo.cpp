#include "falloutttwsavegameinfo.h"

#include "falloutttwsavegame.h"
#include "gamegamebryo.h"

FalloutTTWSaveGameInfo::FalloutTTWSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

FalloutTTWSaveGameInfo::~FalloutTTWSaveGameInfo()
{
}

MOBase::ISaveGame const *FalloutTTWSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new FalloutTTWSaveGame(file, m_Game);
}
