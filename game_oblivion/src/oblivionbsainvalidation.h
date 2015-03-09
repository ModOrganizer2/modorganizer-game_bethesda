#ifndef OBLIVIONBSAINVALIDATION_H
#define OBLIVIONBSAINVALIDATION_H


#include <gamebryobsainvalidation.h>
#include <memory>
#include "obliviondataarchives.h"


class OblivionBSAInvalidation : public GamebryoBSAInvalidation
{
public:

  OblivionBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // OBLIVIONBSAINVALIDATION_H
