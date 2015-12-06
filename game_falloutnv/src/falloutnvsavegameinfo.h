#ifndef FALLOUTNVSAVEGAMEINFO_H
#define FALLOUTNVSAVEGAMEINFO_H

#include "savegameinfo.h"

class FalloutNVSaveGameInfo : public SaveGameInfo
{
public:
  virtual MOBase::ISaveGame const *getSaveGameInfo(QString const &file) const override;
};

#endif // FALLOUTNVSAVEGAMEINFO_H
