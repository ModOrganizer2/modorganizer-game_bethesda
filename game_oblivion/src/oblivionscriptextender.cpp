#include "oblivionscriptextender.h"

#include <QString>
#include <QStringList>

OblivionScriptExtender::OblivionScriptExtender(GameGamebryo const *game) :
  GamebryoScriptExtender(game)
{
}

OblivionScriptExtender::~OblivionScriptExtender()
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
