#include "oblivionsavegameinfo.h"

#include "oblivionsavegame.h"

MOBase::ISaveGame const *OblivionSaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new OblivionSaveGame(file);
}

