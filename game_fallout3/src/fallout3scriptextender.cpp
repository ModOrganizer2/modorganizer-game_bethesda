#include "fallout3scriptextender.h"

#include <QString>
#include <QStringList>

Fallout3ScriptExtender::Fallout3ScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

QString Fallout3ScriptExtender::name() const
{
  return "fose";
}

QStringList Fallout3ScriptExtender::saveGameAttachmentExtensions() const
{
  return { };
}
