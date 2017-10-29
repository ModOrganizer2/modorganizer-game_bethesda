#include "skyrimscriptextender.h"

#include <QString>
#include <QStringList>

SkyrimScriptExtender::SkyrimScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

QString SkyrimScriptExtender::BinaryName() const
{
  return "skse";
}

QString SkyrimScriptExtender::PluginPath() const
{
  return "skse/plugins";
}

QStringList SkyrimScriptExtender::saveGameAttachmentExtensions() const
{
  return { "skse" };
}
