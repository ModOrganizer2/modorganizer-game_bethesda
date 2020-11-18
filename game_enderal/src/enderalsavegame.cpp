#include "enderalsavegame.h"

#include <Windows.h>

#include "gameenderal.h"

EnderalSaveGame::EnderalSaveGame(QString const &fileName, GameEnderal const *game) :
  GamebryoSaveGame(fileName, game)
{
  FileWrapper file(getFilepath(), "TESV_SAVEGAME");

  FILETIME ftime;
  fetchInformationFields(file, m_SaveNumber, m_PCName, m_PCLevel, m_PCLocation, ftime);

  //A file time is a 64-bit value that represents the number of 100-nanosecond
  //intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal Time (UTC).
  //So we need to convert that to something useful
  SYSTEMTIME ctime;
  ::FileTimeToSystemTime(&ftime, &ctime);
  setCreationTime(ctime);
}


void EnderalSaveGame::fetchInformationFields(FileWrapper& file,
  unsigned long& saveNumber,
  QString& playerName,
  unsigned short& playerLevel,
  QString& playerLocation,
  FILETIME& creationTime) const
{
  file.skip<unsigned long>(); // header size
  file.skip<unsigned long>(); // header version
  file.read(saveNumber);

  file.read(playerName);

  unsigned long temp;
  file.read(temp);
  playerLevel = static_cast<unsigned short>(temp);

  file.read(playerLocation);

  QString timeOfDay;
  file.read(timeOfDay);

  QString race;
  file.read(race); // race name (i.e. BretonRace)

  file.skip<unsigned short>(); // Player gender (0 = male)
  file.skip<float>(2); // experience gathered, experience required

  file.read(creationTime);
}

std::unique_ptr<GamebryoSaveGame::DataFields> EnderalSaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), "TESV_SAVEGAME");
  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  {
    QString dummyName, dummyLocation;
    unsigned short dummyLevel;
    unsigned long dummySaveNumber;
    FILETIME dummyTime;

    fetchInformationFields(file, dummySaveNumber, dummyName, dummyLevel,
      dummyLocation, dummyTime);
  }

  fields->Screenshot = file.readImage();

  file.skip<unsigned char>(); // form version
  file.skip<unsigned long>(); // plugin info size

  fields->Plugins = file.readPlugins();

  return fields;
}
