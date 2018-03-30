#include "morrowindsavegame.h"

#include <Windows.h>
#include <QPixmap>
#include <filesystem>

MorrowindSaveGame::MorrowindSaveGame(QString const &fileName, MOBase::IPluginGame const *game) :
  GamebryoSaveGame(fileName, game)
{
  FileWrapper file(this, "TES3");
  file.skip<uint32_t>(3); // data size
  file.skip<unsigned char>(4); // HEDR tag
  file.skip<uint32_t>(); // header size
  file.skip<float>(); // header version
  file.skip<uint32_t>(); // following data chunk size? seems to be 9 groupings of 32 bytes
  file.skip<unsigned char>(32); // Author empty for save files
  std::vector<char> saveName(256); // 31 char save name with a null terminator
  file.read(saveName.data(), 256);
  m_SaveName = QString::fromLatin1(saveName.data(), 256).trimmed(); // The defined save name. This is technically the description, but is likely only 31+\0 chars max.
  file.skip<uint32_t>(); // NumRecords (for the entire save)
  std::vector<char> buffer(255);
  file.read(buffer.data(), 4);
  // Parse the MAST/DATA records
  while (QString::fromLatin1(buffer.data(), 4)=="MAST") {
    uint32_t len;
    file.read(len); // Length of master name
    QString name;
    file.read(buffer.data(), len); // Name of master
    name = QString::fromLatin1(buffer.data(), len - 1);
	  file.skip<unsigned char>(4); // DATA record
    file.read(len); // Length
    file.skip<unsigned char>(len); // Typically size 8 - contains length of master data for version checking
    
    file.read(buffer.data(), 4); // Get next record type
	  this->m_Plugins.push_back(name);
  }
  
  // Start of GMDT
  file.skip<uint32_t>(); // size of record

  file.read(m_PCCurrentHealth);
  file.read(m_PCCMaxHealth);

  file.skip<double>(); // current stam?
  file.skip<double>(); // max stam?
  //file.skip<double>(2); // unknown values

  file.read(buffer.data(), 64);
  m_PCLocation = QString::fromLatin1(buffer.data(), 64).trimmed();

  file.read(m_GameDays);

  file.read(buffer.data(), 32);
  m_PCName=QString::fromLatin1(buffer.data(), 32).trimmed();

  // End of GMDT
  
  file.skip<unsigned char>(28); // Skip the SCRD
  // I believe this tells the engine what color each pixel represents and the bitness of the image

  // Start of screenshot
  file.skip<unsigned char>(4); // SCRS
  file.skip<uint32_t>(); // Size of screenshot always 65536 (128x128x4) RGBA8888
  readImageBGRA(file, 128, 128, 0, 1);
  this->m_Screenshot = this->m_Screenshot.scaled(252,192);

  //definitively have to use another method to access the player level
  //it is stored in the fifth byte of the NPDT subrecord of the first NPC_ record
  
  //Globals, Scripts, Regions
  //file.skip<unsigned char>();
  std::vector<char> buff(4);
  file.read(buff.data(), 4);
  while (QString::fromLatin1(buff.data(), 4) != "NPC_")
  {
    uint32_t len;
    file.read(len);
    file.skip<unsigned char>(8 + len);
    file.read(buff.data(), 4);
  }
  while (QString::fromLatin1(buff.data(), 4) == "NPC_") {
    uint32_t size;
    file.read(size);
    file.skip<unsigned long>(3);
    uint32_t len;
    file.read(len);
    file.read(buffer.data(), len);
    if (QString::fromLatin1(buffer.data(), len - 1) == "player") {
      file.read(buff.data(), 4);
      while (QString::fromLatin1(buff.data(), 4) != "NPDT")
      {
        uint32_t len;
        file.read(len);
        file.skip<unsigned char>(len);
        file.read(buff.data(), 4);
      }
      file.skip<unsigned long>();
      file.read(m_PCLevel);
    }
    else
    {
      file.skip<unsigned char>(size - len - 8);
    }
  }

  std::experimental::filesystem::path realFile(fileName.toStdWString());
  QString realFileName = QString::fromStdWString(realFile.filename().wstring());
  m_SaveNumber = realFileName.mid(4, 5).remove(QRegExp("0+$")).toInt();
}

void MorrowindSaveGame::readImageBGRA(GamebryoSaveGame::FileWrapper &file, unsigned long width, unsigned long height, int scale = 0, bool alpha = false)
{
  QImage image(width, height, QImage::Format_RGBA8888);
  for (unsigned long h = 0; h < width; h++) {
    for (unsigned long w = 0; w < width; w++) {
      uint8_t blue;
      file.read(blue);
      uint8_t green;
      file.read(green);
      uint8_t red;
      file.read(red);
      uint8_t alpha;
      file.read(alpha);
      alpha = 255 - alpha;
      QColor color(red, green, blue, alpha);
      image.setPixel(w, h, color.rgba());
    }
  }
  if (scale != 0)
    m_Screenshot = image.copy().scaledToWidth(scale);
  else
    m_Screenshot = image.copy();
}