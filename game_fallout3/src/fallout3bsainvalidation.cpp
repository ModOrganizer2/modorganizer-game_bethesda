#include "fallout3bsainvalidation.h"

Fallout3BSAInvalidation::Fallout3BSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IPluginGame *game)
  : GamebryoBSAInvalidation(dataArchives, "fallout.ini", game)
{
}

QString Fallout3BSAInvalidation::invalidationBSAName() const
{
  return "Fallout - Invalidation.bsa";
}

unsigned long Fallout3BSAInvalidation::bsaVersion() const
{
  return 0x68;
}
