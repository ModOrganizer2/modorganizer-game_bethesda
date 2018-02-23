#include "morrowindscriptextender.h"

#include <QString>
#include <QStringList>

MorrowindScriptExtender::MorrowindScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

QString MorrowindScriptExtender::BinaryName() const
{
  return "skse_loader.exe";
}

QString MorrowindScriptExtender::PluginPath() const
{
  return "skse/plugins";
}

QStringList MorrowindScriptExtender::saveGameAttachmentExtensions() const
{
  return { "skse" };
}
