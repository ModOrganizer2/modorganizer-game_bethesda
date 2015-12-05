#include "falloutnvsavegameinfo.h"

#include "falloutnvsavegame.h"

MOBase::ISaveGame const *FalloutNVSaveGameInfo::getSaveGameInfo(const QString &file) const
{
  return new FalloutNVSaveGame(file);
}
