#ifndef FALLOUTNVBSAINVALIDATION_H
#define FALLOUTNVBSAINVALIDATION_H


#include <gamebryobsainvalidation.h>
#include <memory>
#include "falloutnvdataarchives.h"


class FalloutNVBSAInvalidation : public GamebryoBSAInvalidation
{
public:

  FalloutNVBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // FALLOUTNVBSAINVALIDATION_H
