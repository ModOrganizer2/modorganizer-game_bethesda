#ifndef MORROWINDDATAARCHIVES_H
#define MORROWINDDATAARCHIVES_H


#include <gamebryodataarchives.h>
#include <iprofile.h>
#include <QString>
#include <QStringList>
#include <QDir>

class MorrowindDataArchives : public GamebryoDataArchives
{

public:
  MorrowindDataArchives(const QDir &myGamesDir);

public:

  virtual QStringList vanillaArchives() const override;
  virtual QStringList archives(const MOBase::IProfile *profile) const override;

protected:

  QStringList getArchives(const QString &iniFile) const
  void setArchives(const QString &iniFile, const QStringList &list)
  
private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) override;

};

#endif // MORROWINDDATAARCHIVES_H
