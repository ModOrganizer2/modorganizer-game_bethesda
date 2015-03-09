#include "oblivionbsainvalidation.h"
#include <imoinfo.h>


OblivionBSAInvalidation::OblivionBSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo)
  : GamebryoBSAInvalidation(dataArchives, "oblivion.ini", moInfo)
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
