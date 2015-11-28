#include "oblivionscriptextender.h"

#include <QString>
#include <QStringList>

OblivionScriptExtender::OblivionScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

QString OblivionScriptExtender::name() const
{
  return "obse";
}

QStringList OblivionScriptExtender::saveGameAttachmentExtensions() const
{
  return { name() };
}
