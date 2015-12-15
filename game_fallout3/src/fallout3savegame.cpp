#include "fallout3savegame.h"

Fallout3SaveGame::Fallout3SaveGame(QString const &fileName, MOBase::IPluginGame const *game) :
  GamebryoSaveGame(fileName, game)
{
  FileWrapper file(this, "FO3SAVEGAME");

  file.skip<unsigned long>(); //Save header size

  file.setHasFieldMarkers(true);

  file.skip<unsigned long>(); //File version ?
  file.skip<unsigned char>(); //delimiter

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

  file.skip<char>(5); // unknown (1 byte), plugin size (4 bytes)

  //Abstract this
  file.readPlugins();
}

