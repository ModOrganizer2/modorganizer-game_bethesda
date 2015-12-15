#include "falloutnvsavegameinfo.h"

#include "falloutnvsavegame.h"

FalloutNVSaveGameInfo::FalloutNVSaveGameInfo(MOBase::IPluginGame const *game) :
  m_Game(game)
{
}

MOBase::ISaveGame const *FalloutNVSaveGameInfo::getSaveGameInfo(QString const &file) const
{
  return new FalloutNVSaveGame(file, m_Game);
}
