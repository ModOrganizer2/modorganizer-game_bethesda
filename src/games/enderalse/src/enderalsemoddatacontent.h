#ifndef ENDERALSE_MODDATACONTENT_H
#define ENDERALSE_MODDATACONTENT_H

#include <gamebryomoddatacontent.h>
#include <ifiletree.h>

class EnderalSEModDataContent : public GamebryoModDataContent {
public:

  /**
   *
   */
  EnderalSEModDataContent(GameGamebryo const* gamePlugin) : GamebryoModDataContent(gamePlugin) {
    // Just need to disable some contents:
    m_Enabled[CONTENT_SKYPROC] = false;
  }

};

#endif // SKYRIMSE_MODDATACONTENT_H
