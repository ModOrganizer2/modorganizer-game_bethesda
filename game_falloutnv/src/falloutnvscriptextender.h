#ifndef FALLOUTNVSCRIPTEXTENDER_H
#define FALLOUTNVSCRIPTEXTENDER_H


#include <scriptextender.h>


class FalloutNVScriptExtender : public ScriptExtender
{
public:
  virtual QString name() const override;

  virtual QString loaderName() const override;

  virtual QStringList saveGameAttachmentExtensions() const override;

};

#endif // FALLOUTNVSCRIPTEXTENDER_H
