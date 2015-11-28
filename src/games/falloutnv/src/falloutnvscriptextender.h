#ifndef FALLOUTNVSCRIPTEXTENDER_H
#define FALLOUTNVSCRIPTEXTENDER_H

#include "gamebryoscriptextender.h"

class FalloutNVScriptExtender : public GamebryoScriptExtender
{
public:
  FalloutNVScriptExtender(const GameGamebryo *game);

  virtual QString name() const override;

  virtual QStringList saveGameAttachmentExtensions() const override;

};

#endif // FALLOUTNVSCRIPTEXTENDER_H
