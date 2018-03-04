#include "falloutnvbsainvalidation.h"

FalloutTTWBSAInvalidation::FalloutNVBSAInvalidation(DataArchives *dataArchives, MOBase::IPluginGame const *game)
  : GamebryoBSAInvalidation(dataArchives, "fallout.ini", game)
{
}

QString FalloutNVBSAInvalidation::invalidationBSAName() const
{
  return "Fallout - Invalidation.bsa";
}

unsigned long FalloutTTWBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
