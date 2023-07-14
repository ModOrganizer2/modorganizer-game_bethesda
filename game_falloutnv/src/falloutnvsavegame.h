#ifndef FALLOUTNVSAVEGAME_H
#define FALLOUTNVSAVEGAME_H

#include "gamebryosavegame.h"

class GameFalloutNV;

class FalloutNVSaveGame : public GamebryoSaveGame
{
public:
  FalloutNVSaveGame(QString const &fileName, GameFalloutNV const *game);

protected:

  // Fetch easy-to-access information.
  void fetchInformationFields(
    FileWrapper& wrapper,
    unsigned long& width,
    unsigned long& height,
    unsigned long& saveNumber,
    QString& playerName,
    unsigned short& playerLevel,
    QString& playerLocation) const;

  std::unique_ptr<DataFields> fetchDataFields() const override;
};

#endif // FALLOUTNVSAVEGAME_H
