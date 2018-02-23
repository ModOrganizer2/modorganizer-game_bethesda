#include "morrowindsavegame.h"

#include <Windows.h>

MorrowindSaveGame::MorrowindSaveGame(QString const &fileName, MOBase::IPluginGame const *game) :
  GamebryoSaveGame(fileName, game)
{
  FileWrapper file(this, "TES3");
  //file.skip<unsigned long>(); // header size
  //file.skip<unsigned long>(); // header version
  file.skip<unsigned long>(79); //Mostly empty header Data
  //file.readPlugins(); normal readPlugins function does not work
  file.skip<unsigned char>();
  uint8_t count;
  file.read(count);
  this->m_Plugins.reserve(count);
  file.skip<unsigned short>();
  for (std::size_t i = 0; i < count; ++i) {
	  file.skip<unsigned long>();
      QString name;
      file.read(name);
	  uint8_t tmp;
	  file.read(tmp);
	  name+=tmp;
	  file.skip<unsigned char>();
	  file.skip<unsigned long>(4);
	  this->m_Plugins.push_back(name);
  }
  
  file.skip<unsigned char>(31);
  file.setBZString(true);
  file.read(m_PCLocation);
  
  file.skip<unsigned char>();
  std::vector<char> buffer(32);
  file.read(buffer.data(), 32);
  
  m_PCName=QString::fromLatin1(buffer.data(), 32);
  
  //file.read(m_PCName);
  //m_PCName="Placeholder";
  
  // Placeholder values
  m_PCLevel=0;
  m_SaveNumber=0;
  
  /*file.skip()
  file.read(m_SaveNumber);

  file.read(m_PCName);

  unsigned long temp;
  file.read(temp);
  m_PCLevel = static_cast<unsigned short>(temp);

  file.read(m_PCLocation);

  QString timeOfDay;
  file.read(timeOfDay);

  QString race;
  file.read(race); // race name (i.e. BretonRace)

  file.skip<unsigned short>(); // Player gender (0 = male)
  file.skip<float>(2); // experience gathered, experience required

  FILETIME ftime;
  file.read(ftime);
  //A file time is a 64-bit value that represents the number of 100-nanosecond
  //intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal Time (UTC).
  //So we need to convert that to something useful
  SYSTEMTIME ctime;
  ::FileTimeToSystemTime(&ftime, &ctime);

  setCreationTime(ctime);

  file.readImage();

  file.skip<unsigned char>(); // form version
  file.skip<unsigned long>(); // plugin info size

  file.readPlugins(); */
}
