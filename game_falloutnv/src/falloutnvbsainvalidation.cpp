#include "falloutnvbsainvalidation.h"
#include <imoinfo.h>


FalloutNVBSAInvalidation::FalloutNVBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo)
  : GamebryoBSAInvalidation(dataArchives, "fallout.ini", moInfo)
{
}

QString FalloutNVBSAInvalidation::invalidationBSAName() const
{
  return "Fallout - Invalidation.bsa";
}

unsigned long FalloutNVBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
