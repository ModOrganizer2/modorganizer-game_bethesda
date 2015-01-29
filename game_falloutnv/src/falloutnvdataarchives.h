#ifndef FALLOUTNVDATAARCHIVES_H
#define FALLOUTNVDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>

class FalloutNVDataArchives : public GamebryoDataArchives
{

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, QStringList before) override;

};

#endif // FALLOUTNVDATAARCHIVES_H
