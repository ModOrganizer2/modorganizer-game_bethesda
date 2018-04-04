#include "skyrimvrunmanagedmods.h"

SkyrimVRUnmangedMods::SkyrimVRUnmangedMods(const GameGamebryo *game)
  : GamebryoUnmangedMods(game)
{}

SkyrimVRUnmangedMods::~SkyrimVRUnmangedMods()
{}

QStringList SkyrimVRUnmangedMods::mods(bool onlyOfficial) const {
  QStringList result;

  QStringList pluginList = game()->primaryPlugins();
  QStringList otherPlugins = game()->DLCPlugins();
  otherPlugins.append(game()->CCPlugins());
  for (QString plugin : otherPlugins) {
    pluginList.removeAll(plugin);
  }
  QDir dataDir(game()->dataDirectory());
  for (const QString &fileName : dataDir.entryList({ "*.esp", "*.esl", "*.esm" })) {
    if (!pluginList.contains(fileName, Qt::CaseInsensitive)) {
      if (!onlyOfficial || pluginList.contains(fileName, Qt::CaseInsensitive)) {
        QFileInfo file(fileName);
        result.append(file.baseName());
      }
    }
  }

  return result;
}
