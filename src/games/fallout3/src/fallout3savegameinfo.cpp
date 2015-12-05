#include "fallout3savegameinfo.h"

#include "fallout3savegame.h"

MOBase::ISaveGame const *Fallout3SaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new Fallout3SaveGame(file);
}

