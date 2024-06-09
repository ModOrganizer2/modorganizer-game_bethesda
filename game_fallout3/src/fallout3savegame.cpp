#include "fallout3savegame.h"

#include "gamefallout3.h"

Fallout3SaveGame::Fallout3SaveGame(QString const& fileName, GameFallout3 const* game)
    : GamebryoSaveGame(fileName, game)
{
  FileWrapper file(getFilepath(), "FO3SAVEGAME");

  unsigned long width, height;
  fetchInformationFields(file, width, height, m_SaveNumber, m_PCName, m_PCLevel,
                         m_PCLocation);
}

void Fallout3SaveGame::fetchInformationFields(FileWrapper& file, unsigned long& width,
                                              unsigned long& height,
                                              unsigned long& saveNumber,
                                              QString& playerName,
                                              unsigned short& playerLevel,
                                              QString& playerLocation) const
{
  file.skip<unsigned long>();  // Save header size

  file.setHasFieldMarkers(true);
  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BZSTRING);

  file.skip<unsigned long>();  // File version ?
  file.skip<unsigned char>();  // delimiter

  file.read(width);
  file.read(height);
  file.read(saveNumber);
  file.read(playerName);

  QString whatthis;
  file.read(whatthis);

  long level;
  file.read(level);
  playerLevel = level;

  file.read(playerLocation);
}

std::unique_ptr<GamebryoSaveGame::DataFields> Fallout3SaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), "FO3SAVEGAME");

  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  unsigned long width, height;
  {
    QString dummyName, dummyLocation;
    unsigned short dummyLevel;
    unsigned long dummySaveNumber;

    fetchInformationFields(file, width, height, dummySaveNumber, dummyName, dummyLevel,
                           dummyLocation);
  }

  QString playtime;
  file.read(playtime);

  fields->Screenshot = file.readImage(width, height, 256);

  file.skip<char>(5);  // unknown (1 byte), plugin size (4 bytes)

  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BSTRING);
  fields->Plugins = file.readPlugins();

  return fields;
}
