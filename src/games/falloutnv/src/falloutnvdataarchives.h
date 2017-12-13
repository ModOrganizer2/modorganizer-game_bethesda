#ifndef FALLOUTNVDATAARCHIVES_H
#define FALLOUTNVDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>
#include <QDir>

class FalloutNVDataArchives : public GamebryoDataArchives
{
public:
  FalloutNVDataArchives(const QDir &myGamesDir);

public:
  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:
  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // FALLOUTNVDATAARCHIVES_H
