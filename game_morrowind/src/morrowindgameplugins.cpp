#include "morrowindgameplugins.h"
#include <Windows.h>
#include <utility.h>
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
using MOBase::reportError;

MorrowindGamePlugins::MorrowindGamePlugins(IOrganizer *organizer) :
  GamebryoGamePlugins(organizer)
{
}

void MorrowindGamePlugins::writePluginLists(const IPluginList *pluginList) {
  if (!m_LastRead.isValid()) {
    // attempt to write uninitialized plugin lists
    return;
  }

  writePluginList(pluginList,
                  organizer()->profile()->absolutePath() + "/Morrowind.ini");
  writeLoadOrderList(pluginList,
                     organizer()->profile()->absolutePath() + "/loadorder.txt");

  m_LastRead = QDateTime::currentDateTime();
}

void MorrowindGamePlugins::readPluginLists(MOBase::IPluginList *pluginList) {
  QString loadOrderPath =
      organizer()->profile()->absolutePath() + "/loadorder.txt";
  QString pluginsPath = organizer()->profile()->absolutePath() + "/Morrowind.ini";

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

void MorrowindGamePlugins::writePluginList(const MOBase::IPluginList *pluginList,
                                          const QString &filePath) {
  return writeList(pluginList, filePath, false);
}

void MorrowindGamePlugins::writeList(const IPluginList *pluginList,
                                    const QString &filePath, bool loadOrder) {
  QTextCodec *textCodec = loadOrder ? utf8Codec() : localCodec();
  
  ::WritePrivateProfileSectionW(L"Game Files", NULL, filePath.toStdWString().c_str());
  
  bool invalidFileNames = false;
  int writtenCount = 0;
  
  QStringList plugins = pluginList->pluginNames();
  std::sort(plugins.begin(), plugins.end(),
            [pluginList](const QString &lhs, const QString &rhs) {
              return pluginList->priority(lhs) < pluginList->priority(rhs);
            });
  QString key = "GameFile";
  for (const QString &pluginName : plugins) {
    if (loadOrder ||
        (pluginList->state(pluginName) == IPluginList::STATE_ACTIVE)) {
      if (!textCodec->canEncode(pluginName)) {
        invalidFileNames = true;
        qCritical("invalid plugin name %s", qPrintable(pluginName));
      } else {
	    if (!::WritePrivateProfileStringW(L"Game Files", (key+QString::number(writtenCount)).toStdWString().c_str(), pluginName.toStdWString().c_str(), filePath.toStdWString().c_str())) {
          throw MOBase::MyException(QObject::tr("failed to set game file key (errorcode %1)").arg(errno));
        }
      }
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
  }
}

bool MorrowindGamePlugins::readPluginList(MOBase::IPluginList *pluginList,
                                          bool useLoadOrder) {
  QStringList primary = organizer()->managedGame()->primaryPlugins();
  for (const QString &pluginName : primary) {
    if (pluginList->state(pluginName) != IPluginList::STATE_MISSING) {
      pluginList->setState(pluginName, IPluginList::STATE_ACTIVE);
    }
  }
  QStringList plugins = pluginList->pluginNames();
  // Do not sort the primary plugins. Their load order should be locked as defined in "primaryPlugins".
  for (QString plugin : plugins) {
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

  QString filePath = organizer()->profile()->absolutePath() + "/Morrowind.ini";
  wchar_t buffer[256];
  QStringList result;
  std::wstring iniFileW = QDir::toNativeSeparators(filePath).toStdWString();
  
  errno = 0;

  QStringList loadOrder;
  QString key = "GameFile";
  int i=0;
  while (::GetPrivateProfileStringW(L"Game Files", (key+QString::number(i)).toStdWString().c_str(),
                                 L"", buffer, 256, iniFileW.c_str()) != 0) {
	QString pluginName;
    pluginName=QString::fromStdWString(buffer).trimmed();
	pluginList->setState(pluginName, IPluginList::STATE_ACTIVE);
    plugins.removeAll(pluginName);
	i++;
  }

  for (const QString &pluginName : plugins) {
    pluginList->setState(pluginName, IPluginList::STATE_INACTIVE);
  }

  return true;
}