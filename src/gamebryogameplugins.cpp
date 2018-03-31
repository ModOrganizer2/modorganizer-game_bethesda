#include "gamebryogameplugins.h"
#include <safewritefile.h>
#include <report.h>
#include <ipluginlist.h>
#include <iplugingame.h>
#include <imodinterface.h>
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
      !QFileInfo(loadOrderPath).exists() ||
      QFileInfo(loadOrderPath).lastModified() > m_LastRead;
  bool pluginsIsNew = !m_LastRead.isValid() ||
      QFileInfo(pluginsPath).lastModified() > m_LastRead;

  if (loadOrderIsNew || !pluginsIsNew) {
    // read both files if they are both new or both older than the last read
    readLoadOrderList(pluginList, loadOrderPath);
    readPluginList(pluginList, false);
  } else {
    // If the plugins is new but not loadorder, we must reparse the load order from the plugin files
    readPluginList(pluginList, true);
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
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        readPluginList(pluginList, true);
    } else {
        QStringList plugins = organizer()->managedGame()->primaryPlugins();

        ON_BLOCK_EXIT([&file]() { file.close(); });

        if (file.size() == 0) {
          readPluginList(pluginList, true);
          return true;
        }
        while (!file.atEnd()) {
            QByteArray line = file.readLine().trimmed();
            QString modName;
            if ((line.size() > 0) && (line.at(0) != '#')) {
                modName = QString::fromUtf8(line.constData()).toLower();
            }

            if (modName.size() > 0) {
                if (!plugins.contains(modName, Qt::CaseInsensitive)) {
                    plugins.append(modName);
                }
            }
        }

        pluginList->setLoadOrder(plugins);
    }

    return true;
}

bool GamebryoGamePlugins::readPluginList(MOBase::IPluginList *pluginList,
                                         bool useLoadOrder) {
  QStringList primary = organizer()->managedGame()->primaryPlugins();
  for (const QString &pluginName : primary) {
      if (pluginList->state(pluginName) != IPluginList::STATE_MISSING) {
          pluginList->setState(pluginName, IPluginList::STATE_ACTIVE);
      }
  }
  QStringList plugins = pluginList->pluginNames();
  QStringList pluginsClone(plugins);
  // Do not sort the primary plugins. Their load order should be locked as defined in "primaryPlugins".
  for (QString plugin : pluginsClone) {
      if (primary.contains(plugin, Qt::CaseInsensitive))
          plugins.removeAll(plugin);
  }

  if (useLoadOrder) {
      // Always use filetime loadorder to get the actual load order
      std::sort(plugins.begin(), plugins.end(), [&](const QString &lhs, const QString &rhs) {
          MOBase::IModInterface *lhm = organizer()->getMod(pluginList->origin(lhs));
          MOBase::IModInterface *rhm = organizer()->getMod(pluginList->origin(rhs));
          QDir lhd = organizer()->managedGame()->dataDirectory();
          QDir rhd = organizer()->managedGame()->dataDirectory();
          if (lhm != nullptr)
              lhd = lhm->absolutePath();
          if (rhm != nullptr)
              rhd = rhm->absolutePath();
          QString lhp = lhd.absoluteFilePath(lhs);
          QString rhp = rhd.absoluteFilePath(rhs);
          return QFileInfo(lhp).lastModified() <
              QFileInfo(rhp).lastModified();
      });

      // Add the primary plugins to the beginning of the load order
      pluginList->setLoadOrder(primary + plugins);
  }

  // Determine plugin active state by the plugins.txt file.
  bool pluginsTxtExists = true;
  QString filePath = organizer()->profile()->absolutePath() + "/plugins.txt";
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
      pluginsTxtExists = false;
  }
  ON_BLOCK_EXIT([&]() {
      qDebug("close %s", qPrintable(filePath));
      file.close();
  });

  if (file.size() == 0) {
      // MO stores at least a header in the file. if it's completely empty the
      // file is broken
      pluginsTxtExists = false;
  }

  if (pluginsTxtExists) {
      while (!file.atEnd()) {
          QByteArray line = file.readLine();
          QString pluginName;
          if ((line.size() > 0) && (line.at(0) != '#')) {
              pluginName = m_LocalCodec->toUnicode(line.trimmed().constData());
          }
          if (pluginName.size() > 0) {
              pluginList->setState(pluginName, IPluginList::STATE_ACTIVE);
              plugins.removeAll(pluginName);
          }
      }

      file.close();

      // we removed each plugin found in the file, so what's left are inactive mods
      for (const QString &pluginName : plugins) {
          pluginList->setState(pluginName, IPluginList::STATE_INACTIVE);
      }
  } else {
      for (const QString &pluginName : plugins) {
          pluginList->setState(pluginName, IPluginList::STATE_INACTIVE);
      }
  }

  return true;
}
