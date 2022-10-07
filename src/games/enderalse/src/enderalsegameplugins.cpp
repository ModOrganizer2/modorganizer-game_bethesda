#include "enderalsegameplugins.h"

#include <ipluginlist.h>
#include <report.h>
#include <log.h>
#include <safewritefile.h>

#include <QStringEncoder>

using namespace MOBase;

void EnderalSEGamePlugins::writePluginList(const MOBase::IPluginList* pluginList, const QString& filePath)
{
  SafeWriteFile file(filePath);

  QStringEncoder encoder(QStringConverter::Encoding::System);

  file->resize(0);

  file->write(encoder.encode(
    "# This file was automatically generated by Mod Organizer.\r\n"));

  bool invalidFileNames = false;
  int writtenCount = 0;

  QStringList plugins = pluginList->pluginNames();
  std::sort(plugins.begin(), plugins.end(),
    [pluginList](const QString& lhs, const QString& rhs) {
      return pluginList->priority(lhs) < pluginList->priority(rhs);
    });

  QStringList PrimaryPlugins = organizer()->managedGame()->primaryPlugins();
  QStringList DLCPlugins = organizer()->managedGame()->DLCPlugins();
  QSet<QString> ManagedMods = QSet<QString>(PrimaryPlugins.begin(), PrimaryPlugins.end());
  QSet<QString> DLCSet = QSet<QString>(DLCPlugins.begin(), DLCPlugins.end());
  ManagedMods.subtract(DLCSet);
  PrimaryPlugins.append(QList<QString>(ManagedMods.begin(), ManagedMods.end()));

  // we need to force some plugins because those are not force-loaded
  // by the game but are considered primary plugins for users
  file->write("*Enderal - Forgotten Stories.esm\r\n");
  file->write("*SkyUI_SE.esp\r\n");

  //TODO: do not write plugins in OFFICIAL_FILES container
  for (const QString& pluginName : plugins) {
    if (!PrimaryPlugins.contains(pluginName, Qt::CaseInsensitive)) {
      if (pluginList->state(pluginName) == IPluginList::STATE_ACTIVE) {
        auto result = encoder.encode(pluginName);
        if (encoder.hasError()) {
          invalidFileNames = true;
          qCritical("invalid plugin name %s", qUtf8Printable(pluginName));
        }
        else
        {
          file->write("*");
          file->write(result);

        }
        file->write("\r\n");
        ++writtenCount;
      }
      else
      {
        auto result = encoder.encode(pluginName);
        if (encoder.hasError()) {
          invalidFileNames = true;
          qCritical("invalid plugin name %s", qUtf8Printable(pluginName));
        }
        else
        {
          file->write(result);
        }
        file->write("\r\n");
        ++writtenCount;
      }
    }
  }

  if (invalidFileNames) {
    reportError(QObject::tr("Some of your plugins have invalid names! These "
      "plugins can not be loaded by the game. Please see "
      "mo_interface.log for a list of affected plugins "
      "and rename them."));
  }

  file.commitIfDifferent(m_LastSaveHash[filePath]);
}

