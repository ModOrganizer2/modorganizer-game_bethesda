#ifndef SKYRIMSCRIPTEXTENDER_H
#define SKYRIMSCRIPTEXTENDER_H

#include "gamebryoscriptextender.h"

class GameGamebryo;

class SkyrimScriptExtender : public GamebryoScriptExtender
{
public:
  SkyrimScriptExtender(const GameGamebryo *game);

  virtual QString name() const override;

  virtual QStringList saveGameAttachmentExtensions() const override;

};

#endif // SKYRIMSCRIPTEXTENDER_H
