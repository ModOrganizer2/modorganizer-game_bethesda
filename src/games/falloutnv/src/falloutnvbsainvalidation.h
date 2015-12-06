#ifndef FALLOUTNVBSAINVALIDATION_H
#define FALLOUTNVBSAINVALIDATION_H


#include "gamebryobsainvalidation.h"
#include "falloutnvdataarchives.h"

#include <memory>

class FalloutNVBSAInvalidation : public GamebryoBSAInvalidation
{
public:

  FalloutNVBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IPluginGame const *game);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // FALLOUTNVBSAINVALIDATION_H
