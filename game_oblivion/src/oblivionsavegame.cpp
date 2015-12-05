#include "oblivionsavegame.h"

#include <Windows.h>

OblivionSaveGame::OblivionSaveGame(const QString &game) :
  GamebryoSaveGame(game)
{
  FileWrapper file(this, "TES4SAVEGAME");
  file.setBZString(true);

  file.skip<unsigned char>(); //Major version
  file.skip<unsigned char>(); //Minor version

  file.skip<SYSTEMTIME>();  // exe last modified (!)

  file.skip<unsigned long>(); //Header version
  file.skip<unsigned long>(); //Header size

  file.read(m_SaveNumber);

  file.read(m_PCName);
  file.read(m_PCLevel);
  file.read(m_PCLocation);

  file.skip<float>(); //game days
  file.skip<unsigned long>(); //game ticks

  //there is a save time stored here. So use it rather than the file time, which
  //could have been copied.
  //Note: This says it uses getlocaltime api to obtain it which is u/s - if so
  //we should ignore this.
  SYSTEMTIME ctime;
  file.read(ctime);
  setCreationTime(ctime);

  //Note that screenshot size, width, height and data are apparently the same
  //structure
  file.skip<unsigned long>(); //Screenshot size.

  file.readImage();

  file.readPlugins();
}
