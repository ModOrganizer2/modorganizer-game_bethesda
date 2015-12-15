#include "skyrimsavegameinfo.h"

#include "skyrimsavegame.h"

SkyrimSaveGameInfo::SkyrimSaveGameInfo(MOBase::IPluginGame const *game) :
  m_Game(game)
{
}


MOBase::ISaveGame const *SkyrimSaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new SkyrimSaveGame(file, m_Game);
}

