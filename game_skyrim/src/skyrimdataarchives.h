#ifndef SKYRIMDATAARCHIVES_H
#define SKYRIMDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>

class SkyrimDataArchives : public GamebryoDataArchives
{

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // SKYRIMDATAARCHIVES_H
