#include "falloutnvsavegame.h"

FalloutNVSaveGame::FalloutNVSaveGame(QString const &fileName) :
  GamebryoSaveGame(fileName)
{
  FileWrapper file(this, "FO3SAVEGAME");

  file.skip<char>();

  char ignore = 0x00;
  while (ignore != 0x7c) {
    file.read(ignore); // unknown
  }
  ignore = 0x00;
  // in new vegas there is another block of uninteresting (?) information
  file.skip<char>(); // 0x7c
  while (ignore != 0x7c) {
    file.read(ignore); // unknown
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

  //Abstract this
  QScopedArrayPointer<unsigned char> buffer(new unsigned char[width * height * 3]);
  file.read(buffer.data(), width * height * 3);
  // why do I have to copy here? without the copy, the buffer seems to get deleted after the
  // temporary vanishes, but Qts implicit sharing should handle that?
  m_Screenshot = QImage(buffer.data(), width, height, QImage::Format_RGB888).scaledToWidth(256);

  file.skip<char>(5); // unknown

  //Abstract this
  file.readPlugins();
}

