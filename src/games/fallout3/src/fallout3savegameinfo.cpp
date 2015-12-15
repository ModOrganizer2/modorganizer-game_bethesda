#include "fallout3savegameinfo.h"

#include "fallout3savegame.h"

Fallout3SaveGameInfo::Fallout3SaveGameInfo(MOBase::IPluginGame const *game) :
  m_Game(game)
{
}

MOBase::ISaveGame const *Fallout3SaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new Fallout3SaveGame(file, m_Game);
}

