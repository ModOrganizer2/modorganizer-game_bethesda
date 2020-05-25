#include "gamebryomoddatacontent.h"

#include <scriptextender.h>

#include "gamegamebryo.h"

std::vector<GamebryoModDataContent::Content> GamebryoModDataContent::getAllContents() const {
  return {
    {CONTENT_PLUGIN,    ":/MO/gui/content/plugin",    QT_TR_NOOP("Game Plugins (ESP/ESM/ESL)")},
    {CONTENT_INTERFACE, ":/MO/gui/content/interface", QT_TR_NOOP("Interface")},
    {CONTENT_MESH,      ":/MO/gui/content/mesh",      QT_TR_NOOP("Meshes")},
    {CONTENT_BSA,       ":/MO/gui/content/bsa",       QT_TR_NOOP("Bethesda Archive")},
    {CONTENT_SCRIPT,    ":/MO/gui/content/script",    QT_TR_NOOP("Scripts (Papyrus)")},
    {CONTENT_SKSE,      ":/MO/gui/content/skse",      QT_TR_NOOP("Script Extender Plugin")},
    {CONTENT_SKYPROC,   ":/MO/gui/content/skyproc",   QT_TR_NOOP("SkyProc Patcher")},
    {CONTENT_SOUND,     ":/MO/gui/content/sound",     QT_TR_NOOP("Sound or Music")},
    {CONTENT_TEXTURE,   ":/MO/gui/content/texture",   QT_TR_NOOP("Textures")},
    {CONTENT_MCM,       ":/MO/gui/content/menu",      QT_TR_NOOP("MCM Configuration")},
    {CONTENT_INI,       ":/MO/gui/content/inifile",   QT_TR_NOOP("INI files")},
    {CONTENT_MODGROUP,  ":/MO/gui/content/modgroup",  QT_TR_NOOP("ModGroup files")}
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