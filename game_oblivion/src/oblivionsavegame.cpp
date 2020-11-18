#include "oblivionsavegame.h"

#include <Windows.h>

OblivionSaveGame::OblivionSaveGame(QString const &fileName, GameOblivion const *game) :
  GamebryoSaveGame(fileName, game)
{
  FileWrapper file(getFilepath(), "TES4SAVEGAME");
  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BSTRING);

  SYSTEMTIME creationTime;
  fetchInformationFields(file, m_SaveNumber, m_PCName, m_PCLevel, m_PCLocation, creationTime);
  setCreationTime(creationTime);
}

void OblivionSaveGame::fetchInformationFields(FileWrapper& file,
  unsigned long& saveNumber,
  QString& playerName,
  unsigned short& playerLevel,
  QString& playerLocation,
  SYSTEMTIME& creationTime) const
{
  file.skip<unsigned char>(); //Major version
  file.skip<unsigned char>(); //Minor version

  file.skip<SYSTEMTIME>();  // exe last modified (!)

  file.skip<unsigned long>(); //Header version
  file.skip<unsigned long>(); //Header size

  file.read(saveNumber);

  file.read(playerName);
  file.read(playerLevel);
  file.read(playerLocation);

  file.skip<float>(); //game days
  file.skip<unsigned long>(); //game ticks

  //there is a save time stored here. So use it rather than the file time, which
  //could have been copied.
  //Note: This says it uses getlocaltime api to obtain it which is u/s - if so
  //we should ignore this.
  file.read(creationTime);
}

std::unique_ptr<GamebryoSaveGame::DataFields> OblivionSaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), "TES4SAVEGAME");
  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BSTRING);

  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  {
    QString dummyName, dummyLocation;
    unsigned short dummyLevel;
    unsigned long dummySaveNumber;
    SYSTEMTIME dummyTime;

    fetchInformationFields(file, dummySaveNumber, dummyName, dummyLevel,
      dummyLocation, dummyTime);
  }

  //Note that screenshot size, width, height and data are apparently the same
  //structure
  file.skip<unsigned long>(); //Screenshot size.

  fields->Screenshot = file.readImage();

  fields->Plugins = file.readPlugins();

  return fields;
}
