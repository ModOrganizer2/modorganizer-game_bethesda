#ifndef FALLOUT4VRDATAARCHIVES_H
#define FALLOUT4VRDATAARCHIVES_H

#include "gamebryodataarchives.h"

namespace MOBase { class IProfile; }

#include <QStringList>
#include <QDir>

class Fallout4VRDataArchives : public GamebryoDataArchives
{

public:

  Fallout4VRDataArchives(const QDir &myGamesDir);

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // Fallout4VRDataArchives_H
