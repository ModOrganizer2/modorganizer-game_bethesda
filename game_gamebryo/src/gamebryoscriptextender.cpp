#include "gamebryoscriptextender.h"

#include "gamegamebryo.h"

#include <QDir>
#include <QString>

GamebryoScriptExtender::GamebryoScriptExtender(const GameGamebryo *game) :
  m_Game(game)
{
}

GamebryoScriptExtender::~GamebryoScriptExtender()
{
}

QString GamebryoScriptExtender::loaderName() const
{
  return BinaryName();
}

QString GamebryoScriptExtender::loaderPath() const
{
  return m_Game->gameDirectory().absoluteFilePath(loaderName());
}

bool GamebryoScriptExtender::isInstalled() const
{
  //A note: It is possibly also OK if xxse_steam_loader.dll exists, but it's
  //not clear why that would exist and the exe not if you'd installed it per
  //instructions, and it'd mess up NCC installs a treat.
  return m_Game->gameDirectory().exists(loaderName());

}

QString GamebryoScriptExtender::getExtenderVersion() const
{
  return m_Game->getVersion(loaderName());
}

