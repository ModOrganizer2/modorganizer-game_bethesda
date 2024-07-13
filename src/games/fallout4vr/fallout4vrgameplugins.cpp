#include "fallout4vrgameplugins.h"

using namespace MOBase;

Fallout4VRGamePlugins::Fallout4VRGamePlugins(MOBase::IOrganizer* organizer)
    : CreationGamePlugins(organizer)
{}

bool Fallout4VRGamePlugins::lightPluginsAreSupported()
{
  return false;
}
