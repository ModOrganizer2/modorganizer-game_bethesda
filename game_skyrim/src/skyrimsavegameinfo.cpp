#include "skyrimsavegameinfo.h"

#include "skyrimsavegame.h"

MOBase::ISaveGame const *SkyrimSaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new SkyrimSaveGame(file);
}

