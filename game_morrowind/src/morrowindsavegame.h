#ifndef MORROWINDSAVEGAME_H
#define MORROWINDSAVEGAME_H

#include "gamebryosavegame.h"

namespace MOBase { class IPluginGame; }

class MorrowindSaveGame : public GamebryoSaveGame
{
public:
  MorrowindSaveGame(QString const &fileName, MOBase::IPluginGame const *game);

  //Simple getters
  QString getSaveName() const { return m_SaveName; }
  float getPCCurrentHealth() const { return m_PCCurrentHealth; }
  float getPCMaxHealth() const { return m_PCCMaxHealth; }
  float getGameDays() const { return m_GameDays; }

protected:
  QString m_SaveName;
  float m_PCCurrentHealth;
  float m_PCCMaxHealth;
  float m_GameDays;
};

#endif // MORROWINDSAVEGAME_H
