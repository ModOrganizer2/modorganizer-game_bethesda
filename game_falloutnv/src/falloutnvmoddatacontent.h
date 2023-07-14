#ifndef FALLOUTNV_MODDATACONTENT_H
#define FALLOUTNV_MODDATACONTENT_H

#include <gamebryomoddatacontent.h>
#include <ifiletree.h>

class FalloutNVModDataContent : public GamebryoModDataContent
{
public:
  /**
   *
   */
  FalloutNVModDataContent(GameGamebryo const* gamePlugin)
      : GamebryoModDataContent(gamePlugin)
  {
    // Just need to disable some contents:
    m_Enabled[CONTENT_MCM]     = false;
    m_Enabled[CONTENT_SKYPROC] = false;
  }
};

#endif  // FALLOUTNV_MODDATACONTENT_H
