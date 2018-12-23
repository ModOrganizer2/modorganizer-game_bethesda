#ifndef FALLOUT76DATAARCHIVES_H
#define FALLOUT76DATAARCHIVES_H

#include "gamebryodataarchives.h"

namespace MOBase { class IProfile; }

#include <QStringList>
#include <QDir>

class Fallout76DataArchives : public GamebryoDataArchives
{

public:

  Fallout76DataArchives(const QDir &myGamesDir);

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList Fallout76DataArchives::sResourceIndexFileList() const;
  virtual QStringList Fallout76DataArchives::sResourceStartUpArchiveList() const;
  virtual QStringList Fallout76DataArchives::SResourceArchiveMemoryCacheList() const;
  virtual QStringList Fallout76DataArchives::SResourceArchiveList() const;
  virtual QStringList Fallout76DataArchives::SResourceArchiveList2() const;
  virtual QStringList Fallout76DataArchives::sResourceArchive2List() const;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // FALLOUT76DATAARCHIVES_H
