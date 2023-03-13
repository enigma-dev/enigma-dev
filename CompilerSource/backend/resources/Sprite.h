/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_SPRITE_H
#define ENIGMA_SPRITE_H

#include "backend/JavaStruct.h"
#include "backend/sub/SubImage.h"
#include "backend/util/Polygon.h"

namespace deprecated {
namespace JavaStruct {

struct Sprite
{
  String name;
  int id;

  boolean transparent;
  int shape; //0*=Precise, 1=Rectangle,  2=Disk, 3=Diamond
  int alphaTolerance;
  boolean separateMask;
  boolean smoothEdges;
  boolean preload;
  int originX;
  int originY;

  int bbMode; //0*=Automatic, 1=Full image, 2=Manual
  int bbLeft;
  int bbRight;
  int bbTop;
  int bbBottom;

  SubImage *subImages;
  int subImageCount;

  Polygon_LOLWINDOWS *maskShapes;
  int maskShapeCount;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
