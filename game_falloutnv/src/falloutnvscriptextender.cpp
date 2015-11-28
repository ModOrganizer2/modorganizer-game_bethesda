#include "falloutnvscriptextender.h"

#include <QString>
#include <QStringList>

QString FalloutNVScriptExtender::name() const
{
  return "nvse";
}

QString FalloutNVScriptExtender::loaderName() const
{
  return name() + "_loader.exe";
}

QStringList FalloutNVScriptExtender::saveGameAttachmentExtensions() const
{
  return { };
}
