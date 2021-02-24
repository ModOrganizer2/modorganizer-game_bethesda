#ifndef ENDERALSEBSAINVALIDATION_H
#define ENDERALSEBSAINVALIDATION_H


#include "gamebryobsainvalidation.h"
#include "enderalsedataarchives.h"

#include <memory>

class EnderalSEBSAInvalidation : public GamebryoBSAInvalidation
{
public:

  EnderalSEBSAInvalidation(DataArchives* dataArchives, MOBase::IPluginGame const* game);

private:

  virtual QString invalidationBSAName() const override;
  virtual unsigned long bsaVersion() const override;

};

#endif // ENDERALSEBSAINVALIDATION_H