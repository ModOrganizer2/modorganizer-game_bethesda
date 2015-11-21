#include "oblivionbsainvalidation.h"


OblivionBSAInvalidation::OblivionBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IPluginGame *game)
  : GamebryoBSAInvalidation(dataArchives, "oblivion.ini", game)
{
}

QString OblivionBSAInvalidation::invalidationBSAName() const
{
  return "Oblivion - Invalidation.bsa";
}

unsigned long OblivionBSAInvalidation::bsaVersion() const
{
  return 0x67;
}
