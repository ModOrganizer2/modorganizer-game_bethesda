#include "morrowindsavegameinfo.h"

#include "morrowindsavegame.h"
#include "gamegamebryo.h"

MorrowindSaveGameInfo::MorrowindSaveGameInfo(GameGamebryo const *game) :
  GamebryoSaveGameInfo(game)
{
}

MorrowindSaveGameInfo::~MorrowindSaveGameInfo()
{
}


MOBase::ISaveGame const *MorrowindSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new MorrowindSaveGame(file, m_Game);
}
