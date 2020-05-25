#include "gamebryomoddatacontent.h"

#include <scriptextender.h>

#include "gamegamebryo.h"

std::vector<GamebryoModDataContent::Content> GamebryoModDataContent::getAllContents() const {
  return {
    {CONTENT_PLUGIN,    QT_TR_NOOP("Game Plugins (ESP/ESM/ESL)"), ":/MO/gui/content/plugin"},
    {CONTENT_INTERFACE, QT_TR_NOOP("Interface"),                  ":/MO/gui/content/interface"},
    {CONTENT_MESH,      QT_TR_NOOP("Meshes"),                     ":/MO/gui/content/mesh"},
    {CONTENT_BSA,       QT_TR_NOOP("Bethesda Archive"),           ":/MO/gui/content/bsa"},
    {CONTENT_SCRIPT,    QT_TR_NOOP("Scripts (Papyrus)"),          ":/MO/gui/content/script"},
    {CONTENT_SKSE,      QT_TR_NOOP("Script Extender Plugin"),     ":/MO/gui/content/skse"},
    {CONTENT_SKYPROC,   QT_TR_NOOP("SkyProc Patcher"),            ":/MO/gui/content/skyproc"},
    {CONTENT_SOUND,     QT_TR_NOOP("Sound or Music"),             ":/MO/gui/content/sound"},
    {CONTENT_TEXTURE,   QT_TR_NOOP("Textures"),                   ":/MO/gui/content/texture"},
    {CONTENT_MCM,       QT_TR_NOOP("MCM Configuration"),          ":/MO/gui/content/menu"},
    {CONTENT_INI,       QT_TR_NOOP("INI files"),                  ":/MO/gui/content/inifile"},
    {CONTENT_MODGROUP,  QT_TR_NOOP("ModGroup files"),             ":/MO/gui/content/modgroup"}
  };
}

std::vector<int> GamebryoModDataContent::getContentsFor(std::shared_ptr<const MOBase::IFileTree> fileTree) const {
  std::vector<int> contents;

  for (auto e : *fileTree) {
    if (e->isFile()) {
      auto suffix = e->suffix().toLower();
      if (suffix == "esp" || suffix == "esm" || suffix == "esl") {
        contents.push_back(CONTENT_PLUGIN);
      }
      else if (suffix == "bsa" || suffix == "ba2") {
        contents.push_back(CONTENT_BSA);
      }
      else if (suffix == "ini" && e->compare("meta.ini") != 0) {
        contents.push_back(CONTENT_INI);
      }
      else if (suffix == "modgroups") {
        contents.push_back(CONTENT_MODGROUP);
      }
    }
    else {
      if (e->compare("textures") == 0 || e->compare("icons") == 0 || e->compare("bookart") == 0)
        contents.push_back(CONTENT_TEXTURE);
      if (e->compare("meshes") == 0)
        contents.push_back(CONTENT_MESH);
      if (e->compare("interface") == 0 || e->compare("menus") == 0)
        contents.push_back(CONTENT_INTERFACE);
      if (e->compare("music") == 0 || e->compare("sound") == 0)
        contents.push_back(CONTENT_SOUND);
      if (e->compare("scripts") == 0)
        contents.push_back(CONTENT_SCRIPT);
      if (e->compare("SkyProc Patchers") == 0)
        contents.push_back(CONTENT_SKYPROC);
      if (e->compare("MCM") == 0)
        contents.push_back(CONTENT_MCM);
    }
  }

  ScriptExtender* extender = m_GamePlugin->feature<ScriptExtender>();
  if (extender != nullptr) {
    auto e = fileTree->findDirectory(extender->PluginPath());
    if (e) {
      for (auto f : *e) {
        if (f->hasSuffix("dll")) {
          contents.push_back(CONTENT_SKSE);
          break;
        }
      }
    }
  }

  return contents;
}