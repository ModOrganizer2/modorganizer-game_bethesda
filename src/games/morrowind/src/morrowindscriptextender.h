#ifndef MORROWINDSCRIPTEXTENDER_H
#define MORROWINDSCRIPTEXTENDER_H

#include "gamebryoscriptextender.h"

class GameGamebryo;

class MorrowindScriptExtender : public GamebryoScriptExtender
{
public:
  MorrowindScriptExtender(const GameGamebryo *game);

  virtual QString BinaryName() const override;
  virtual QString PluginPath() const override;

  virtual QStringList saveGameAttachmentExtensions() const override;

};

#endif // MORROWINDSCRIPTEXTENDER_H
