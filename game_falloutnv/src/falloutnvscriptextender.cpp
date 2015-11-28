#include "falloutnvscriptextender.h"

#include <QString>
#include <QStringList>

FalloutNVScriptExtender::FalloutNVScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

QString FalloutNVScriptExtender::name() const
{
  return "nvse";
}

QStringList FalloutNVScriptExtender::saveGameAttachmentExtensions() const
{
  return { };
}
