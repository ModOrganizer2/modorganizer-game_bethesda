#ifndef FALLOUT4SCRIPTEXTENDER_H
#define FALLOUT4SCRIPTEXTENDER_H

#include "gamebryoscriptextender.h"

class GameGamebryo;

class Fallout4LondonScriptExtender : public GamebryoScriptExtender
{
public:
  Fallout4LondonScriptExtender(GameGamebryo const* game);

  virtual QString BinaryName() const override;
  virtual QString PluginPath() const override;
};

#endif  // FALLOUT4SCRIPTEXTENDER_H
