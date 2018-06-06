#include "falloutttwsavegame.h"

FalloutTTWSaveGame::FalloutTTWSaveGame(QString const &fileName, MOBase::IPluginGame const *game) :
  GamebryoSaveGame(fileName, game)
{
  FileWrapper file(this, "FO3SAVEGAME");

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

  file.skip<char>(5); // unknown byte, size of plugin data

  //Abstract this
  file.setPluginString(GamebryoSaveGame::StringType::TYPE_BSTRING);
  file.readPlugins();
}
