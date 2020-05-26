#include "gamebryomoddatacontent.h"

#include <scriptextender.h>

#include "gamegamebryo.h"

GamebryoModDataContent::GamebryoModDataContent(GameGamebryo const* gamePlugin) : 
  m_GamePlugin(gamePlugin), m_Enabled(CONTENT_MODGROUP, true) { }

std::vector<GamebryoModDataContent::Content> GamebryoModDataContent::getAllContents() const {
  static std::vector<Content> GAMEBRYO_CONTENTS{
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

  // Copy the list of enabled contents:
  std::vector<Content> contents;
  std::copy_if(std::begin(GAMEBRYO_CONTENTS), std::end(GAMEBRYO_CONTENTS),
    std::back_inserter(contents), [this](auto e) { return m_Enabled[e.id()]; });
  return contents;
}

std::vector<int> GamebryoModDataContent::getContentsFor(std::shared_ptr<const MOBase::IFileTree> fileTree) const {
  std::vector<int> contents;

  for (auto e : *fileTree) {
    if (e->isFile()) {
      auto suffix = e->suffix().toLower();
      if (m_Enabled[CONTENT_PLUGIN] && (suffix == "esp" || suffix == "esm" || suffix == "esl")) {
        contents.push_back(CONTENT_PLUGIN);
      }
      else if (m_Enabled[CONTENT_BSA] && (suffix == "bsa" || suffix == "ba2")) {
        contents.push_back(CONTENT_BSA);
      }
      else if (m_Enabled[CONTENT_INI] && suffix == "ini" && e->compare("meta.ini") != 0) {
        contents.push_back(CONTENT_INI);
      }
      else if (m_Enabled[CONTENT_MODGROUP] && suffix == "modgroups") {
        contents.push_back(CONTENT_MODGROUP);
      }
    }
    else {
      if (m_Enabled[CONTENT_TEXTURE] && (e->compare("textures") == 0 || e->compare("icons") == 0 || e->compare("bookart") == 0)) {
        contents.push_back(CONTENT_TEXTURE);
      }
      else if (m_Enabled[CONTENT_MESH] && e->compare("meshes") == 0) {
        contents.push_back(CONTENT_MESH);
      }
      else if (m_Enabled[CONTENT_INTERFACE] && (e->compare("interface") == 0 || e->compare("menus") == 0)) {
        contents.push_back(CONTENT_INTERFACE);
      }
      else if (m_Enabled[CONTENT_SOUND] && e->compare("music") == 0 || e->compare("sound") == 0) {
        contents.push_back(CONTENT_SOUND);
      }
      else if (m_Enabled[CONTENT_SCRIPT] && e->compare("scripts") == 0) {
        contents.push_back(CONTENT_SCRIPT);
      }
      else if (m_Enabled[CONTENT_SKYPROC] && e->compare("SkyProc Patchers") == 0) {
        contents.push_back(CONTENT_SKYPROC);
      }
      else if (m_Enabled[CONTENT_MCM] && e->compare("MCM") == 0) {
        contents.push_back(CONTENT_MCM);
      }
    }
  }

  ScriptExtender* extender = m_GamePlugin->feature<ScriptExtender>();
  if (m_Enabled[CONTENT_SKSE] && extender != nullptr) {
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