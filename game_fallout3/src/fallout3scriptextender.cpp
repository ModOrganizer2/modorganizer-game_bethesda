#include "fallout3scriptextender.h"

#include <QString>
#include <QStringList>

QString Fallout3ScriptExtender::name() const
{
  return "fose";
}

QString Fallout3ScriptExtender::loaderName() const
{
  return name() + "_loader.exe";
}

QStringList Fallout3ScriptExtender::saveGameAttachmentExtensions() const
{
  return { };
}
