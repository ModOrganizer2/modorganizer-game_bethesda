#ifndef GAMEBRYOSAVEGAMEINFO_H
#define GAMEBRYOSAVEGAMEINFO_H

#include "savegameinfo.h"

class GameGamebryo;

class GamebryoSaveGameInfo : public SaveGameInfo
{
public:
  GamebryoSaveGameInfo(GameGamebryo const *game);
  ~GamebryoSaveGameInfo();

  virtual MissingAssets getMissingAssets(QString const &file) const override;

  virtual MOBase::ISaveGameInfoWidget *getSaveGameWidget(QWidget *) const override;

  virtual bool hasScriptExtenderSave(QString const &file) const override;

protected:
  friend class GamebryoSaveGameInfoWidget;
  GameGamebryo const *m_Game;
};

#endif // GAMEBRYOSAVEGAMEINFO_H
