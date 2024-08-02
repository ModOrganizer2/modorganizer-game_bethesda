#ifndef FALLOUT4SAVEGAME_H
#define FALLOUT4SAVEGAME_H

#include "gamebryosavegame.h"

#include <Windows.h>

class GameFallout4London;

class Fallout4LondonSaveGame : public GamebryoSaveGame
{
public:
  Fallout4LondonSaveGame(QString const& fileName, GameFallout4London const* game);

protected:
  // Fetch easy-to-access information.
  void fetchInformationFields(FileWrapper& file, unsigned long& saveNumber,
                              QString& playerName, unsigned short& playerLevel,
                              QString& playerLocation, FILETIME& creationTime) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif  // FALLOUT4SAVEGAME_H
