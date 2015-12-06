#ifndef OBLIVIONSAVEGAMEINFO_H
#define OBLIVIONSAVEGAMEINFO_H

#include "savegameinfo.h"

class OblivionSaveGameInfo : public SaveGameInfo
{
public:
  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // OBLIVIONSAVEGAMEINFO_H
