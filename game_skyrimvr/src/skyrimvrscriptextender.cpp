#include "skyrimvrscriptextender.h"

#include <QString>
#include <QStringList>

SkyrimVRScriptExtender::SkyrimVRScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

QString SkyrimVRScriptExtender::BinaryName() const
{
  return "skse64_loader.exe";
}

QString SkyrimVRScriptExtender::PluginPath() const
{
  return "skse/plugins";
}

QStringList SkyrimVRScriptExtender::saveGameAttachmentExtensions() const
{
  return { };
}
