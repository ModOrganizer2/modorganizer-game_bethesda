#include "oblivionsavegameinfo.h"

#include "oblivionsavegame.h"

OblivionSaveGameInfo::OblivionSaveGameInfo(MOBase::IPluginGame const *game) :
  m_Game(game)
{
}

MOBase::ISaveGame const *OblivionSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new OblivionSaveGame(file, m_Game);
}

