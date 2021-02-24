#ifndef ENDERALSESAVEGAME_H
#define ENDERALSESAVEGAME_H

#include "gamebryosavegame.h"
#include "gameenderalse.h"

namespace MOBase { class IPluginGame; }

class EnderalSESaveGame : public GamebryoSaveGame
{
public:
  EnderalSESaveGame(QString const &fileName, GameEnderalSE const *game);

protected:

  // Fetch easy-to-access information.
  void fetchInformationFields(FileWrapper& wrapper,
    unsigned long& version,
    QString& playerName,
    unsigned short& playerLevel,
    QString& playerLocation,
    unsigned long& saveNumber,
    FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;

};

#endif // _SKYRIMSESAVEGAME_H
