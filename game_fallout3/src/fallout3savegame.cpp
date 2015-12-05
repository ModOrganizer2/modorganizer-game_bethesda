#include "fallout3savegame.h"

Fallout3SaveGame::Fallout3SaveGame(QString const &fileName) :
  GamebryoSaveGame(fileName)
{
  FileWrapper file(this, "FO3SAVEGAME");

  file.skip<char>();

  char ignore = 0x00;
  while (ignore != 0x7c) {
    file.read(ignore); // unknown
  }
  bool newVegas = false;
  if (newVegas) {
    ignore = 0x00;
    // in new vegas there is another block of uninteresting (?) information
    file.skip<char>(); // 0x7c
    while (ignore != 0x7c) {
      file.read(ignore); // unknown
    }
  }

  file.setHasFieldMarkers(true);

  unsigned long width;
  file.read(width);

  unsigned long height;
  file.read(height);

  file.read(m_SaveNumber);

  file.read(m_PCName);

  QString whatthis;
  file.read(whatthis);

  long level;
  file.read(level);
  m_PCLevel = level;

  file.read(m_PCLocation);

  QString playtime;
  file.read(playtime);

  file.readImage(width, height, 256);

  file.skip<char>(5); // unknown

  //Abstract this
  file.readPlugins();
}

