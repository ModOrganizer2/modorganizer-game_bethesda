#ifndef SKYRIMBSAINVALIDATION_H
#define SKYRIMBSAINVALIDATION_H


#include "gamebryobsainvalidation.h"
#include "skyrimdataarchives.h"

#include <memory>

class SkyrimBSAInvalidation : public GamebryoBSAInvalidation
{
public:

  SkyrimBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IPluginGame *game);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // SKYRIMBSAINVALIDATION_H
