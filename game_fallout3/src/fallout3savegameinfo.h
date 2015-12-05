#ifndef FALLOUT3SAVEGAMEINFO_H
#define FALLOUT3SAVEGAMEINFO_H

#include "savegameinfo.h"

class Fallout3SaveGameInfo : public SaveGameInfo
{
public:
  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // FALLOUT3SAVEGAMEINFO_H
