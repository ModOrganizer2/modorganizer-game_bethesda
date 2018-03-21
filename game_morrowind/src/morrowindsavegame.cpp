#include "morrowindsavegame.h"

#include <Windows.h>
#include <QPixmap>

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
  std::vector<char> buffer(255);
  file.skip<unsigned short>();
  file.read(buffer.data(), 4);
  while(QString::fromLatin1(buffer.data(), 4)=="MAST"){
	  uint32_t len;
	  file.read(len);
      QString name;
      file.read(buffer.data(), len-1);
	  name=QString::fromLatin1(buffer.data(), len-1);
	  file.skip<unsigned char>();
	  file.skip<unsigned long>(4);
	  file.read(buffer.data(), 4);
	  this->m_Plugins.push_back(name);
  }
  
  file.skip<unsigned long>(7);
  file.read(buffer.data(), 64);
  m_PCLocation=QString::fromLatin1(buffer.data(), 64).trimmed();
  
  file.skip<unsigned long>();
  file.read(buffer.data(), 32);
  
  m_PCName=QString::fromLatin1(buffer.data(), 32).trimmed();
  
  file.skip<unsigned char>(36);
  
  file.readImage(128, 128, 0, 1);
  
  //Color correction, I am unable to get it to work in a more efficient way
  this->m_Screenshot=this->m_Screenshot.rgbSwapped();
  unsigned int rgb;

  for(int y=0;y<this->m_Screenshot.height();y++){
    for(int x=0;x<this->m_Screenshot.width();x++){
      rgb=this->m_Screenshot.pixel(x,y);
      this->m_Screenshot.setPixel(x,y,qRgba(qRed(rgb),qGreen(rgb),qBlue(rgb),255));
    }
  }
  this->m_Screenshot=this->m_Screenshot.scaled(252,192);
  //definitively have to use another method to access the player level
  //it is stored in the fifth byte of the NPDT subrecord of the first NPC_ record
  
  //file.skip<unsigned long>(8); //Record SCRD
  //file.skip<unsigned long>(16385); //Record SCRS
  //file.skip<unsigned char>(8445); //Globals
  
  //Globals, Scripts, Regions
  //file.skip<unsigned char>();
  std::vector<char> buff(4);
  file.read(buff.data(), 4);
  while(QString::fromLatin1(buff.data(), 4)!="NPC_")
  {
    uint32_t len;
	file.read(len);
	file.skip<unsigned char>(8+len);
	file.read(buff.data(), 4);
  }
  while(QString::fromLatin1(buff.data(), 4)=="NPC_"){
    uint32_t size;
    file.read(size);
    file.skip<unsigned long>(3);
	uint32_t len;
	file.read(len);
	file.read(buffer.data(), len);
	if(QString::fromLatin1(buffer.data(), len-1)=="player"){
	  file.read(buff.data(), 4);
	  while(QString::fromLatin1(buff.data(), 4)!="NPDT")
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
	  file.skip<unsigned char>(size-len-8);
	}
  }
  m_SaveNumber=fileName.chopped(4).right(4).toInt();
}
