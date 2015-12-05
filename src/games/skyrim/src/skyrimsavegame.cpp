#include "skyrimsavegame.h"

SkyrimSaveGame::SkyrimSaveGame(QString const &fileName) :
  GamebryoSaveGame(fileName)
{
  FileWrapper file(this, "TESV_SAVEGAME");
  file.skip<unsigned long>(); // header size
  file.skip<unsigned long>(); // header version, -> 8
  file.read(m_SaveNumber);

  file.read(m_PCName);

  unsigned long temp;
  file.read(temp);
  m_PCLevel = static_cast<unsigned short>(temp);

  file.read(m_PCLocation);

  QString playTime;
  file.read(playTime);

  QString race;
  file.read(race); // race name (i.e. BretonRace)

  file.skip<unsigned short>(); // ???
  file.skip<float>(2); // ???
  //FIXME If this is a system time read it and use it as the creation time
  file.skip<unsigned char>(8); // filetime

  unsigned long width, height;
  file.read(width); // 320
  file.read(height); // 192

  QScopedArrayPointer<unsigned char> buffer(new unsigned char[width * height * 3]);
  file.read(buffer.data(), width * height * 3);
  // why do I have to copy here? without the copy, the buffer seems to get deleted after the
  // temporary vanishes, but Qts implicit sharing should handle that?
  m_Screenshot = QImage(buffer.data(), width, height, QImage::Format_RGB888).copy();

  file.skip<unsigned char>(); // form version
  file.skip<unsigned long>(); // plugin info size

  file.readPlugins();
}
