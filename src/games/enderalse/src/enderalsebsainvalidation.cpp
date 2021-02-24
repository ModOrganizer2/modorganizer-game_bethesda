#include "enderalsebsainvalidation.h"

EnderalSEBSAInvalidation::EnderalSEBSAInvalidation(DataArchives* dataArchives, MOBase::IPluginGame const* game)
  : GamebryoBSAInvalidation(dataArchives, "skyrim.ini", game)
{
}

QString EnderalSEBSAInvalidation::invalidationBSAName() const
{
  return "Enderal - Invalidation.bsa";
}

unsigned long EnderalSEBSAInvalidation::bsaVersion() const
{
  return 0x69;
}