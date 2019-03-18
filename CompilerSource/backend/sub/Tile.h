/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_TILE_H
#define ENIGMA_TILE_H

#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

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
/*  int xscale;
  int yscale;
  double alpha;
  int color;*/
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
