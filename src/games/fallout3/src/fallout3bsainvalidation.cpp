#include "fallout3bsainvalidation.h"
#include <imoinfo.h>


Fallout3BSAInvalidation::Fallout3BSAInvalidation(const std::shared_ptr<DataArchives> &dataArchives, MOBase::IOrganizer *moInfo)
  : GamebryoBSAInvalidation(dataArchives, "fallout.ini", moInfo)
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
