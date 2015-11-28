#include "skyrimscriptextender.h"

#include <QString>
#include <QStringList>

QString SkyrimScriptExtender::name() const
{
  return "skse";
}

QString SkyrimScriptExtender::loaderName() const
{
  return name() + "_loader.exe";
}

QStringList SkyrimScriptExtender::saveGameAttachmentExtensions() const
{
  return { name() };
}
