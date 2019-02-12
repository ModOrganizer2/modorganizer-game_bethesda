#ifndef ENDERALDATAARCHIVES_H
#define ENDERALDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>
#include <QDir>

class EnderalDataArchives : public GamebryoDataArchives
{

public:
  EnderalDataArchives(const QDir &myGamesDir);

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // ENDERALDATAARCHIVES_H
