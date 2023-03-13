/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_GAMEINFORMATION_H
#define ENIGMA_GAMEINFORMATION_H

#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct GameInformation
{
  int backgroundColor;
  boolean embedGameWindow;
  String formCaption;
  int left;
  int top;
  int width;
  int height;
  boolean showBorder;
  boolean allowResize;
  boolean stayOnTop;
  boolean pauseGame;
  String gameInfoStr;

  // Default backgroundColor is the same as Game Maker
  GameInformation(): backgroundColor(0xFFFFE100), embedGameWindow(false), formCaption("") {

  }

};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
