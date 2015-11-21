#include "skyrimbsainvalidation.h"

SkyrimBSAInvalidation::SkyrimBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IPluginGame *game)
  : GamebryoBSAInvalidation(dataArchives, "skyrim.ini", game)
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
