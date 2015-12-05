#include "oblivionsavegame.h"

#include <Windows.h>

OblivionSaveGame::OblivionSaveGame(const QString &game) :
  GamebryoSaveGame(game)
{
  FileWrapper file(this, "TES4SAVEGAME");
  file.setStringLength(1);

  file.skip<unsigned char>();

  //Hmm. We could probably just skip all of these
  unsigned char version_minor;
  file.read(version_minor);
  file.skip<SYSTEMTIME>();
  unsigned long headerVersion;
  file.read(headerVersion);
  unsigned long saveHeaderSize;
  file.read(saveHeaderSize);

  file.read(m_SaveNumber);

  file.read(m_PCName);
  file.read(m_PCLevel);
  file.read(m_PCLocation);

  file.skip<float>(); //game days
  file.skip<unsigned long>(); //game ticks

  //there is a save time stored here. So use it rather than the file time, which
  //could have been copied.
  SYSTEMTIME ctime;
  file.read(ctime);
  setCreationTime(ctime);

  unsigned long size;
  file.read(size);

  file.readImage();

  file.readPlugins();
}
