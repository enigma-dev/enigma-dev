/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef _TILE__H
#define _TILE__H

#include "backend/JavaStruct.h"

struct Tile
{
  int bgX;
  int bgY;
  int roomX;
  int roomY;
  int width;
  int height;
  int depth;
  int backgroundId;
  int id;
  boolean locked;
};

#endif
