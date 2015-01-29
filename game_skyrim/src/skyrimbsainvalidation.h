#ifndef SKYRIMBSAINVALIDATION_H
#define SKYRIMBSAINVALIDATION_H


#include <gamebryobsainvalidation.h>
#include <memory>
#include "skyrimdataarchives.h"


class SkyrimBSAInvalidation : public GamebryoBSAInvalidation
{
public:

  SkyrimBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // SKYRIMBSAINVALIDATION_H
