#include "oblivionscriptextender.h"

#include <QString>
#include <QStringList>

QString OblivionScriptExtender::name() const
{
  return "obse";
}

QString OblivionScriptExtender::loaderName() const
{
  return name() + "_loader.exe";
}

QStringList OblivionScriptExtender::saveGameAttachmentExtensions() const
{
  return { name() };
}
