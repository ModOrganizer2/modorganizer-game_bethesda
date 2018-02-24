#include "morrowindsavegame.h"

#include <Windows.h>

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
  file.skip<unsigned short>();
  for (std::size_t i = 0; i < count; ++i) {
	  file.skip<unsigned long>();
      QString name;
      file.read(name);
	  uint8_t tmp;
	  file.read(tmp);
	  name+=tmp;
	  file.skip<unsigned char>();
	  file.skip<unsigned long>(4);
	  this->m_Plugins.push_back(name);
  }
  
  file.skip<unsigned char>(31);
  file.setBZString(true);
  file.read(m_PCLocation);
  
  file.skip<unsigned char>();
  std::vector<char> buffer(32);
  file.read(buffer.data(), 32);
  
  m_PCName=QString::fromLatin1(buffer.data(), 32);
  
  file.skip<unsigned long>(8); //Record SCRD
  file.skip<unsigned long>(16385); //Record SCRS
  //file.skip<unsigned char>(8445); //Globals
  
  //Globals, Scripts, Regions
  std::vector<char> buffer(4);
  file.read(buffer.data(), 4);
  while(QString::fromLatin1(buffer.data(), 4)=="GLOB"||QString::fromLatin1(buffer.data(), 4)=="SCPT"||QString::fromLatin1(buffer.data(), 4)=="REGN")
  {
    uint8_t len;
	file.read(len);
	file.skip<unsigned char>(11+len);
	std::vector<char> buffer(4);
	file.read(buffer.data(), 4);
  }
  
  file.skip<unsigned long>(4);
  
  std::vector<char> buffer(4);
  file.read(buffer.data(), 4);
  while(QString::fromLatin1(buffer.data(), 4)=="NAME"||QString::fromLatin1(buffer.data(), 4)=="FNAME"||QString::fromLatin1(buffer.data(), 4)=="RNAM"||QString::fromLatin1(buffer.data(), 4)=="CNAM"||QString::fromLatin1(buffer.data(), 4)=="ANAM"||QString::fromLatin1(buffer.data(), 4)=="BNAM"||QString::fromLatin1(buffer.data(), 4)=="KNAM")
  {
    uint8_t len;
	file.read(len);
	file.skip<unsigned char>(11+len);
	std::vector<char> buffer(4);
	file.read(buffer.data(), 4);
  }
  
  file.skip<unsigned char>(7);
  file.read(m_PCLevel);
  
  //file.read(m_PCName);
  //m_PCName="Placeholder";
  
  m_SaveNumber=fileName.chop(4).right(4).toInt();
}
