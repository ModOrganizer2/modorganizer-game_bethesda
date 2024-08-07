#ifndef FO4LONDON_MODATACHECKER_H
#define FO4LONDON_MODATACHECKER_H

#include <gamebryomoddatachecker.h>

class Fallout4LondonModDataChecker : public GamebryoModDataChecker
{
public:
  using GamebryoModDataChecker::GamebryoModDataChecker;

protected:
  virtual const FileNameSet& possibleFolderNames() const override
  {
    static FileNameSet result{
        "interface",     "meshes",    "music", "scripts",     "sound", "strings",
        "textures",      "trees",     "video", "materials",   "f4se",  "distantlod",
        "asi",           "Tools",     "MCM",   "distantland", "mits",  "dllplugins",
        "CalienteTools", "shadersfx", "aaf"};
    return result;
  }
  virtual const FileNameSet& possibleFileExtensions() const override
  {
    static FileNameSet result{"esp",       "esm", "esl", "ba2",
                              "modgroups", "ini", "csg", "cdx"};
    return result;
  }
};

#endif  // FO4LONDON_MODATACHECKER_H
