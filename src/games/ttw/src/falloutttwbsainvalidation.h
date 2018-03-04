#ifndef FALLOUTTTWBSAINVALIDATION_H
#define FALLOUTTTWBSAINVALIDATION_H


#include "gamebryobsainvalidation.h"
#include "falloutttwdataarchives.h"

#include <memory>

class FalloutTTWBSAInvalidation : public GamebryoBSAInvalidation
{
public:

  FalloutTTWBSAInvalidation(DataArchives *dataArchives, MOBase::IPluginGame const *game);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // FALLOUTTTWBSAINVALIDATION_H
