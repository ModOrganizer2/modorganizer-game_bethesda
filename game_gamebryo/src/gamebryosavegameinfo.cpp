#include "gamebryosavegameinfo.h"

#include "gamebryosavegame.h"
#include "gamebryosavegameinfowidget.h"
#include "gamegamebryo.h"
#include "imoinfo.h"
#include "imodinterface.h"
#include "iplugingame.h"
#include "ipluginlist.h"

#include <QDir>

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
    if (organizerCore->pluginList()->state(pluginName) != MOBase::IPluginList::STATE_ACTIVE) {
      missingAssets[pluginName] = ProvidingModules();
    }
  }

  // figure out, for each esp/esm, which mod, if any, contains it
  QStringList espFilter( { "*.esp", "*.esm" } );

  // search in data.
  //FIXME DO not search in data.
  {
    QDir dataDir(organizerCore->managedGame()->dataDirectory());
    QStringList esps = dataDir.entryList(espFilter);
    for (const QString &esp : esps) {
      MissingAssets::iterator iter = missingAssets.find(esp);
      if (iter != missingAssets.end()) {
        iter->push_back("<data>");
      }
    }
  }

  //Search normal mods. A note: This will also find mods in data.
  //FIXME Foreign mods should use the right name.
  for (QString const &mod : organizerCore->modsSortedByProfilePriority()) {
    MOBase::IModInterface *modInfo = organizerCore->getMod(mod);
    QStringList esps = QDir(modInfo->absolutePath()).entryList(espFilter);
    for (QString const &esp : esps) {
      MissingAssets::iterator iter = missingAssets.find(esp);
      if (iter != missingAssets.end()) {
        iter->push_back(modInfo->name());
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
        iter->push_back("<overwrite>");
      }
    }
  }

  return missingAssets;
}

MOBase::ISaveGameInfoWidget *GamebryoSaveGameInfo::getSaveGameWidget(QWidget *parent) const
{
  return new GamebryoSaveGameInfoWidget(this, parent);
}
