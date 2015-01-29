#include "skyrimbsainvalidation.h"
#include <imoinfo.h>


SkyrimBSAInvalidation::SkyrimBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo)
  : GamebryoBSAInvalidation(dataArchives, "skyrim.ini", moInfo)
{
}

QString SkyrimBSAInvalidation::invalidationBSAName() const
{
  return "Skyrim - Invalidation.bsa";
}

unsigned long SkyrimBSAInvalidation::bsaVersion() const
{
  return 0x68;
}
