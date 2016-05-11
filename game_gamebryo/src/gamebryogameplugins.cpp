#include "gamebryogameplugins.h"
#include <safewritefile.h>
#include <report.h>
#include <ipluginlist.h>
#include <iplugingame.h>
#include <scopeguard.h>

#include <QDir>
#include <QTextCodec>
#include <QStringList>
#include <QString>
#include <QDateTime>

using MOBase::IOrganizer;
using MOBase::IPluginList;
using MOBase::SafeWriteFile;
using MOBase::reportError;

GamebryoGamePlugins::GamebryoGamePlugins(IOrganizer *organizer)
    : m_Organizer(organizer) {
  m_Utf8Codec = QTextCodec::codecForName("utf-8");
  m_LocalCodec = QTextCodec::codecForName("Windows-1252");
}

void GamebryoGamePlugins::writePluginLists(const IPluginList *pluginList) {
  if (!m_LastRead.isValid()) {
    // attempt to write uninitialized plugin lists
    return;
  }

  writePluginList(pluginList,
                  m_Organizer->profile()->absolutePath() + "/plugins.txt");
  writeLoadOrderList(pluginList,
                     m_Organizer->profile()->absolutePath() + "/loadorder.txt");

  m_LastRead = QDateTime::currentDateTime();
}

void GamebryoGamePlugins::readPluginLists(MOBase::IPluginList *pluginList) {
  QString loadOrderPath =
      organizer()->profile()->absolutePath() + "/loadorder.txt";
  QString pluginsPath = organizer()->profile()->absolutePath() + "/plugins.txt";

  bool loadOrderIsNew = !m_LastRead.isValid() ||
      QFileInfo(loadOrderPath).lastModified() > m_LastRead;
  bool pluginsIsNew = !m_LastRead.isValid() ||
      QFileInfo(pluginsPath).lastModified() > m_LastRead;

  if (loadOrderIsNew || !pluginsIsNew) {
    // read both files if they are both new or both older than the last read
    readLoadOrderList(pluginList, loadOrderPath);
    readPluginList(pluginList, pluginsPath, false);
  } else {
    // if the plugin list is new but the load order isn't, this probably means
    // an external tool that handles only the plugins.txt has been run in the
    // meantime. We have to use plugins.txt for the load order as well.
    readPluginList(pluginList, pluginsPath, true);
  }

  m_LastRead = QDateTime::currentDateTime();
}

void GamebryoGamePlugins::writePluginList(const MOBase::IPluginList *pluginList,
                                          const QString &filePath) {
  return writeList(pluginList, filePath, false);
}

void GamebryoGamePlugins::writeLoadOrderList(
    const MOBase::IPluginList *pluginList, const QString &filePath) {
  return writeList(pluginList, filePath, true);
}

void GamebryoGamePlugins::writeList(const IPluginList *pluginList,
                                    const QString &filePath, bool loadOrder) {
  SafeWriteFile file(filePath);

  QTextCodec *textCodec = loadOrder ? utf8Codec() : localCodec();

  file->resize(0);

  file->write(textCodec->fromUnicode(
      "# This file was automatically generated by Mod Organizer.\r\n"));

  bool invalidFileNames = false;
  int writtenCount = 0;

  QStringList plugins = pluginList->pluginNames();
  std::sort(plugins.begin(), plugins.end(),
            [pluginList](const QString &lhs, const QString &rhs) {
              return pluginList->priority(lhs) < pluginList->priority(rhs);
            });

  for (const QString &pluginName : plugins) {
    if (loadOrder ||
        (pluginList->state(pluginName) == IPluginList::STATE_ACTIVE)) {
      if (!textCodec->canEncode(pluginName)) {
        invalidFileNames = true;
        qCritical("invalid plugin name %s", qPrintable(pluginName));
      } else {
        file->write(textCodec->fromUnicode(pluginName));
      }
      file->write("\r\n");
      ++writtenCount;
    }
  }

  if (invalidFileNames) {
    reportError(QObject::tr("Some of your plugins have invalid names! These "
                            "plugins can not be loaded by the game. Please see "
                            "mo_interface.log for a list of affected plugins "
                            "and rename them."));
  }

  if (writtenCount == 0) {
    qWarning("plugin list would be empty, this is almost certainly wrong. Not "
             "saving.");
  } else {
    if (file.commitIfDifferent(m_LastSaveHash[filePath])) {
      qDebug("%s saved", qPrintable(QDir::toNativeSeparators(filePath)));
    }
  }
}

bool GamebryoGamePlugins::readLoadOrderList(MOBase::IPluginList *pluginList,
                                            const QString &filePath) {
  QStringList pluginNames = organizer()->managedGame()->primaryPlugins();
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    // no load order stored, determine by date
    pluginNames = pluginList->pluginNames();
    QDir dataDirectory = organizer()->managedGame()->dataDirectory();
    std::sort(
        pluginNames.begin(), pluginNames.end(),
        [&dataDirectory](const QString &lhs, const QString &rhs) {
          return QFileInfo(dataDirectory.absoluteFilePath(lhs)).lastModified() >
                 QFileInfo(dataDirectory.absoluteFilePath(rhs)).lastModified();
        });
  } else {
    ON_BLOCK_EXIT([&file]() { file.close(); });

    if (file.size() == 0) {
      // MO stores at least a header in the file. if it's completely empty the
      // file is broken
      return false;
    }
    while (!file.atEnd()) {
      QByteArray line = file.readLine().trimmed();
      QString modName;
      if ((line.size() > 0) && (line.at(0) != '#')) {
        modName = QString::fromUtf8(line.constData()).toLower();
      }

      if (modName.size() > 0) {
        if (!pluginNames.contains(modName, Qt::CaseInsensitive)) {
          pluginNames.append(modName);
        }
      }
    }
  }
  pluginList->setLoadOrder(pluginNames);

  return true;
}

bool GamebryoGamePlugins::readPluginList(MOBase::IPluginList *pluginList,
                                         const QString &filePath,
                                         bool useLoadOrder) {
  QStringList plugins = pluginList->pluginNames();

  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    return false;
  }
  ON_BLOCK_EXIT([&]() {
    qDebug("close %s", qPrintable(filePath));
    file.close();
  });

  if (file.size() == 0) {
    // MO stores at least a header in the file. if it's completely empty the
    // file is broken
    return false;
  }

  QStringList loadOrder;
  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    QString pluginName;
    if ((line.size() > 0) && (line.at(0) != '#')) {
      pluginName = m_LocalCodec->toUnicode(line.trimmed().constData());
    }
    if (pluginName.size() > 0) {
      pluginList->setState(pluginName, IPluginList::STATE_ACTIVE);
      plugins.removeAll(pluginName);
      loadOrder.append(pluginName);
    }
  }

  file.close();

  // we removed each plugin found in the file, so what's left are inactive mods
  for (const QString &pluginName : plugins) {
    pluginList->setState(pluginName, IPluginList::STATE_INACTIVE);
  }

  if (useLoadOrder) {
    pluginList->setLoadOrder(loadOrder);
  }

  return true;
}
