#ifndef NEHRIMDATAARCHIVES_H
#define NEHRIMDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>
#include <QDir>

class NehrimDataArchives : public GamebryoDataArchives
{

public:
  NehrimDataArchives(const QDir &myGamesDir);

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // NEHRIMDATAARCHIVES_H
