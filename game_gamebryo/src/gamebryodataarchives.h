#ifndef GAMEBRYODATAARCHIVES_H
#define GAMEBRYODATAARCHIVES_H


#include <dataarchives.h>


class GamebryoDataArchives : public DataArchives
{

public:

  virtual void addArchive(MOBase::IProfile *profile, int index, const QString &archiveName) override;
  virtual void removeArchive(MOBase::IProfile *profile, const QString &archiveName) override;

protected:

  QStringList getArchivesFromKey(const QString &iniFile, const QString &key) const;
  void setArchivesToKey(const QString &iniFile, const QString &key, const QString &value);

private:

  virtual void writeArchiveList(MOBase::IProfile *profile, const QStringList &before) = 0;

};

#endif // GAMEBRYODATAARCHIVES_H
