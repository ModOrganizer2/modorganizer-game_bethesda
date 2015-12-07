#ifndef GAMEBRYOSCRIPTEXTENDER_H
#define GAMEBRYOSCRIPTEXTENDER_H

#include "scriptextender.h"

class GameGamebryo;

class GamebryoScriptExtender : public ScriptExtender
{
public:
  GamebryoScriptExtender(GameGamebryo const *game);

  virtual ~GamebryoScriptExtender();

  //virtual QString name() const override;

  virtual QString loaderName() const override;

  virtual QString loaderPath() const override;

  //virtual QStringList saveGameAttachmentExtensions() const override;

  virtual bool isInstalled() const override;

  virtual QString getExtenderVersion() const override;

protected:
  GameGamebryo const * const m_Game;
};

#endif // GAMEBRYOSCRIPTEXTENDER_H
