#ifndef FALLOUT4UNMANAGEDMODS_H
#define FALLOUT4UNMANAGEDMODS_H

#include "gamebryounmanagedmods.h"
#include <gamegamebryo.h>

class Fallout4LondonUnmangedMods : public GamebryoUnmangedMods
{
public:
  Fallout4LondonUnmangedMods(const GameGamebryo* game);
  ~Fallout4LondonUnmangedMods();

  virtual QStringList mods(bool onlyOfficial) const override;
  virtual QStringList secondaryFiles(const QString& modName) const override;
  virtual QString displayName(const QString& modName) const override;
};

#endif  // FALLOUT4UNMANAGEDMODS_H
