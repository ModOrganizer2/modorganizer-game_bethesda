#include "starfieldunmanagedmods.h"

#include "log.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

StarfieldUnmanagedMods::StarfieldUnmanagedMods(const GameStarfield* game,
                                               const QString& appDataFolder)
    : GamebryoUnmangedMods(game), m_Game(game), m_AppDataFolder(appDataFolder)
{}

StarfieldUnmanagedMods::~StarfieldUnmanagedMods() {}

QStringList StarfieldUnmanagedMods::mods(bool onlyOfficial) const
{
  QStringList result;

  QStringList pluginList   = game()->primaryPlugins() + game()->enabledPlugins();
  QStringList otherPlugins = game()->DLCPlugins();
  otherPlugins.append(game()->CCPlugins());
  for (QString plugin : otherPlugins) {
    pluginList.removeAll(plugin);
  }
  QMap<QString, QDir> directories = {{"data", game()->dataDirectory()}};
  auto contentCatalog             = parseContentCatalog();
  directories.insert(game()->secondaryDataDirectories());
  for (QDir directory : directories) {
    for (const QString& fileName : directory.entryList({"*.esp", "*.esl", "*.esm"})) {
      if (!pluginList.contains(fileName, Qt::CaseInsensitive)) {
        if (!onlyOfficial || pluginList.contains(fileName, Qt::CaseInsensitive)) {
          bool found = false;
          for (const auto& mod : contentCatalog) {
            if (mod.second.files.contains(fileName, Qt::CaseInsensitive)) {
              result.append(mod.second.name);
              found = true;
            }
          }
          if (!found && !fileName.startsWith("blueprintships-", Qt::CaseInsensitive)) {
            result.append(fileName.chopped(4));  // trims the extension off
          }
        }
      }
    }
  }
  result.removeDuplicates();
  return result;
}

QFileInfo StarfieldUnmanagedMods::referenceFile(const QString& modName) const
{
  QStringList modFiles;
  auto contentCatalog = parseContentCatalog();
  if (contentCatalog.contains(modName)) {
    modFiles = contentCatalog[modName].files;
  }
  QDir dataDir        = m_Game->secondaryDataDirectories().first();
  QStringList plugins = modFiles.filter(
      QRegularExpression("\\.es(m|p|l)$", QRegularExpression::CaseInsensitiveOption));
  QMap<QString, QDir> directories = {{"data", game()->dataDirectory()}};
  directories.insert(game()->secondaryDataDirectories());
  QFileInfoList pluginFiles;
  QFileInfoList files;
  for (QDir directory : directories) {
    if (!plugins.isEmpty())
      pluginFiles += dataDir.entryInfoList(plugins);
    files += directory.entryInfoList(QStringList() << modName + ".es*");
  }
  if (!pluginFiles.isEmpty()) {
    return pluginFiles.at(0);
  } else if (!files.isEmpty()) {
    return files.at(0);
  } else {
    return QFileInfo();
  }
}

std::map<QString, StarfieldUnmanagedMods::ContentCatalog>
StarfieldUnmanagedMods::parseContentCatalog() const
{
  QFile content(m_AppDataFolder + "/" + game()->gameShortName() +
                "/ContentCatalog.txt");
  std::map<QString, StarfieldUnmanagedMods::ContentCatalog> contentCatalog;
  if (content.open(QIODevice::OpenModeFlag::ReadOnly)) {
    auto contentData      = content.readAll();
    QString convertedData = QString::fromLatin1(contentData);
    contentData           = convertedData.toUtf8();
    QJsonParseError jsonError;
    QJsonDocument contentDoc = QJsonDocument::fromJson(contentData, &jsonError);
    if (jsonError.error) {
      MOBase::log::warn(QObject::tr("ContentCatalog.txt appears to be corrupt: %1")
                            .arg(jsonError.errorString()));
    } else {
      QJsonObject contentObj = contentDoc.object();
      for (const auto& mod : contentObj.keys()) {
        if (mod == "ContentCatalog")
          continue;
        auto modInfo = contentObj.value(mod).toObject();
        QStringList pluginList;
        QStringList files;
        for (const auto& file : modInfo.value("Files").toArray()) {
          QString fileName = file.toString();
          files.append(fileName);
          if (fileName.endsWith(".esm", Qt::CaseInsensitive) ||
              fileName.endsWith(".esl", Qt::CaseInsensitive) ||
              fileName.endsWith(".esp", Qt::CaseInsensitive)) {
            pluginList.append(fileName);
          }
        }
        QString name               = modInfo.value("Title").toString();
        contentCatalog[name]       = ContentCatalog();
        contentCatalog[name].files = files;
        contentCatalog[name].name  = name;
      }
    }
  }
  return contentCatalog;
}

QStringList StarfieldUnmanagedMods::secondaryFiles(const QString& modName) const
{
  QStringList files;
  auto contentCatalog = parseContentCatalog();
  if (contentCatalog.contains(modName)) {
    return contentCatalog[modName].files;
  }
  // file extension in FO4 is .ba2 instead of bsa
  QMap<QString, QDir> directories = {{"data", game()->dataDirectory()}};
  directories.insert(game()->secondaryDataDirectories());
  for (QDir directory : directories) {
    for (const QString& archiveName :
         directory.entryList({modName + "*.ba2", "blueprintships-" + modName + ".esm",
                              "blueprintships-" + modName + "*.ba2"})) {
      files.append(directory.absoluteFilePath(archiveName));
    }
  }
  return files;
}

QString StarfieldUnmanagedMods::displayName(const QString& modName) const
{
  auto contentCatalog = parseContentCatalog();
  if (contentCatalog.contains(modName)) {
    return contentCatalog[modName].name;
  }
  if (modName == "ShatteredSpace")
    return "Shattered Space";
  else if (modName == "SFBGS050")
    return "Terran Armada";
  return modName;
}
