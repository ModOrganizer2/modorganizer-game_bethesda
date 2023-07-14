#ifndef SKYRIMVR_MODDATACONTENT_H
#define SKYRIMVR_MODDATACONTENT_H

#include <gamebryomoddatacontent.h>
#include <ifiletree.h>

class SkyrimVRModDataContent : public GamebryoModDataContent {
public:

  /**
   *
   */
  SkyrimVRModDataContent(GameGamebryo const* gamePlugin) : GamebryoModDataContent(gamePlugin) {
    // Just need to disable some contents:
    m_Enabled[CONTENT_SKYPROC] = false;
  }

};

#endif // SKYRIMVR_MODDATACONTENT_H
