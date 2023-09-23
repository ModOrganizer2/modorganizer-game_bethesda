#ifndef ENDERALSELOCALSAVEGAMES_H
#define ENDERALSELOCALSAVEGAMES_H

#include <localsavegames.h>

#include <QDir>
#include <QString>

class EnderalSELocalSavegames : public LocalSavegames
{

public:
  EnderalSELocalSavegames(const QDir& myGamesDir, const QString& iniFileName);

  virtual MappingType mappings(const QDir& profileSaveDir) const override;
  virtual bool prepareProfile(MOBase::IProfile* profile) override;

private:
  QDir m_LocalSavesDir;
  QDir m_LocalGameDir;
  QString m_IniFileName;
};

#endif  // ENDERALSELOCALSAVEGAMES_H
