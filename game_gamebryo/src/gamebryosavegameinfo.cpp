#include "gamebryosavegameinfo.h"

#include "gamebryosavegame.h"
#include "gamebryosavegameinfowidget.h"
#include "gamegamebryo.h"
#include "imoinfo.h"
#include "imodinterface.h"
#include "iplugingame.h"
#include "ipluginlist.h"

#include <QDir>
#include <QString>
#include <QStringList>

GamebryoSaveGameInfo::GamebryoSaveGameInfo(GameGamebryo const *game) :
  m_Game(game)
{
}

GamebryoSaveGameInfo::~GamebryoSaveGameInfo()
{
}

GamebryoSaveGameInfo::MissingAssets GamebryoSaveGameInfo::getMissingAssets(QString const &file) const
{
  GamebryoSaveGame const *save = dynamic_cast<GamebryoSaveGame const *>(getSaveGameInfo(file));
  MOBase::IOrganizer *organizerCore = m_Game->m_Organizer;

  // collect the list of missing plugins
  MissingAssets missingAssets;

  for (QString const &pluginName : save->getPlugins()) {
    switch (organizerCore->pluginList()->state(pluginName)) {
      case MOBase::IPluginList::STATE_INACTIVE:
        missingAssets[pluginName] = ProvidingModules { organizerCore->pluginList()->origin(pluginName) };
        break;
      case MOBase::IPluginList::STATE_MISSING:
        missingAssets[pluginName] = ProvidingModules();
        break;
    }
  }

  //Find out any other mods that might contain the esp/esm
  QStringList espFilter( { "*.esp", "*.esm" } );

  QString dataDir(organizerCore->managedGame()->dataDirectory().absolutePath());

  //Search normal mods. A note: This will also find mods in data.
  for (QString const &mod : organizerCore->modsSortedByProfilePriority()) {
    MOBase::IModInterface *modInfo = organizerCore->getMod(mod);
    QStringList esps = QDir(modInfo->absolutePath()).entryList(espFilter);
    for (QString const &esp : esps) {
      MissingAssets::iterator iter = missingAssets.find(esp);
      if (modInfo->absolutePath() == dataDir) {
        //We have to prune esps that reside in the data directory, otherwise
        //you get all the unmanaged mods listed as potential candidates for
        //enabling
        if (modInfo->name() != organizerCore->pluginList()->origin(esp)) {
          continue;
        }
      }
      if (iter != missingAssets.end()) {
        if (!iter->contains(modInfo->name())) {
          iter->push_back(modInfo->name());
        }
      }
    }
  }

  // search in overwrite
  {
    QDir overwriteDir(organizerCore->overwritePath());
    QStringList esps = overwriteDir.entryList(espFilter);
    for (const QString &esp : esps) {
      MissingAssets::iterator iter = missingAssets.find(esp);
      if (iter != missingAssets.end()) {
        if (!iter->contains("<overwrite>")) {
          iter->push_back("<overwrite>");
        }
      }
    }
  }

  return missingAssets;
}

MOBase::ISaveGameInfoWidget *GamebryoSaveGameInfo::getSaveGameWidget(QWidget *parent) const
{
  return new GamebryoSaveGameInfoWidget(this, parent);
}
