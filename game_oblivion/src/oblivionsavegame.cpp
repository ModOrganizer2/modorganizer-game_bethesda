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

  SYSTEMTIME ctime;
  file.read(ctime);
  //FIXME update creation time with this

  unsigned long size;
  file.read(size);

  unsigned long width;
  file.read(width);
  unsigned long height;
  file.read(height);

  QScopedArrayPointer<unsigned char> buffer(new unsigned char[width * height * 3]);
  file.read(buffer.data(), width * height * 3);
  // why do I have to copy here? without the copy, the buffer seems to get deleted after the
  // temporary vanishes, but Qts implicit sharing should handle that?
  m_Screenshot = QImage(buffer.data(), width, height, QImage::Format_RGB888).copy();

  file.readPlugins();
}
