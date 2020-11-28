#include "falloutnvsavegame.h"

#include "gamefalloutnv.h"

FalloutNVSaveGame::FalloutNVSaveGame(QString const &fileName, GameFalloutNV const *game) :
  GamebryoSaveGame(fileName, game)
{
  FileWrapper file(getFilepath(), "FO3SAVEGAME");
  unsigned long width, height;
  fetchInformationFields(file, width, height, m_SaveNumber, m_PCName, m_PCLevel, m_PCLocation);

}

void FalloutNVSaveGame::fetchInformationFields(
  FileWrapper& file,
  unsigned long& width,
  unsigned long& height,
  unsigned long& saveNumber,
  QString& playerName,
  unsigned short& playerLevel,
  QString& playerLocation) const
{
  file.skip<unsigned long>(); //Save header size

  file.skip<unsigned long>(); //File version?
  file.skip<unsigned char>(); //Delimiter

  //A huge wodge of text with no length but a delimiter. Given the null bytes
  //in it I presume it's fixed length (64 bytes + delim) but I have no
  //definite spec
  for (unsigned char ignore = 0; ignore != 0x7c; ) {
    file.read(ignore); // unknown
  }

  file.setHasFieldMarkers(true);
  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BZSTRING);

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

std::unique_ptr<GamebryoSaveGame::DataFields> FalloutNVSaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), "FO3SAVEGAME");

  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  unsigned long width, height;
  {
    QString dummyName, dummyLocation;
    unsigned short dummyLevel;
    unsigned long dummySaveNumber;

    fetchInformationFields(file, width, height,
      dummySaveNumber, dummyName, dummyLevel, dummyLocation);
  }

  QString playtime;
  file.read(playtime);

  fields->Screenshot = file.readImage(width, height, 256);

  file.skip<char>(5); // unknown (1 byte), plugin size (4 bytes)

  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BSTRING);
  fields->Plugins = file.readPlugins();

  return fields;
}
