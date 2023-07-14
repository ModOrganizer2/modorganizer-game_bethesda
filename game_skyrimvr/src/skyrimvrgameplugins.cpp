#include "skyrimvrgameplugins.h"

using namespace MOBase;

SkyrimVRGamePlugins::SkyrimVRGamePlugins(MOBase::IOrganizer* organizer) : CreationGamePlugins(organizer)
{
}

bool SkyrimVRGamePlugins::lightPluginsAreSupported()
{
    return false;
}