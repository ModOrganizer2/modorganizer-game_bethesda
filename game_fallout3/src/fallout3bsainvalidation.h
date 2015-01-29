#ifndef FALLOUT3BSAINVALIDATION_H
#define FALLOUT3BSAINVALIDATION_H


#include <gamebryobsainvalidation.h>
#include <memory>
#include "fallout3dataarchives.h"


class Fallout3BSAInvalidation : public GamebryoBSAInvalidation
{
public:

  Fallout3BSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // FALLOUT3BSAINVALIDATION_H
