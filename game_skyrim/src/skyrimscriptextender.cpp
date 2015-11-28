#include "skyrimscriptextender.h"

#include <QString>
#include <QStringList>

SkyrimScriptExtender::SkyrimScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

QString SkyrimScriptExtender::name() const
{
  return "skse";
}

QStringList SkyrimScriptExtender::saveGameAttachmentExtensions() const
{
  return { name() };
}
