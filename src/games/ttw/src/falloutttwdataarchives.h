#ifndef FALLOUTTTWDATAARCHIVES_H
#define FALLOUTTTWDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>
#include <QDir>

class FalloutTTWDataArchives : public GamebryoDataArchives
{
public:
  FalloutTTWDataArchives(const QDir &myGamesDir);

public:
  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

private:
  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // FALLOUTTTWDATAARCHIVES_H
