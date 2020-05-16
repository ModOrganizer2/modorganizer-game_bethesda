#include <ifiletree.h>

#include "gamebryomoddatachecker.h"

const QStringList GamebryoModDataChecker::STANDARD_FOLDERS = { 
  "fonts", "interface", "menus", "meshes", "music", "scripts", "shaders",
  "sound", "strings", "textures", "trees", "video", "facegen", "materials",
  "skse", "obse", "mwse", "nvse", "fose", "f4se", "distantlod", "asi",
  "SkyProc Patchers", "Tools", "MCM", "icons", "bookart", "distantland",
  "mits", "splash", "dllplugins", "CalienteTools", "NetScriptFramework",
  "shadersfx"
};

const QStringList GamebryoModDataChecker::STANDARD_EXTENSIONS = {
  "esp", "esm", "esl", "bsa", "ba2", ".modgroups"
};

GamebryoModDataChecker::GamebryoModDataChecker(const GameGamebryo* game, QStringList folders, QStringList extensions) :
  m_Game(game), m_FolderNames(folders.begin(), folders.end()), m_FileExtensions(extensions.begin(), extensions.end()) { }

QString GamebryoModDataChecker::getDataFolderName() const {
  return "data";
}

bool GamebryoModDataChecker::dataLooksValid(std::shared_ptr<const MOBase::IFileTree> fileTree) const {
  for (auto entry : *fileTree) {
    if (entry->isDir()) {
      if (m_FolderNames.count(entry->name()) > 0) {
        return true;
      }
    }
    else {
      if (m_FileExtensions.count(entry->suffix()) > 0) {
        return true;
      }
    }
  }
  return false;
}