#ifndef SKYRIMSCRIPTEXTENDER_H
#define SKYRIMSCRIPTEXTENDER_H


#include <scriptextender.h>


class SkyrimScriptExtender : public ScriptExtender
{
public:
  virtual QString name() const override;

  virtual QString loaderName() const override;

  virtual QStringList saveGameAttachmentExtensions() const override;

};

#endif // SKYRIMSCRIPTEXTENDER_H
