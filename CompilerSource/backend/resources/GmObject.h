/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_GMOBJECT_H
#define ENIGMA_GMOBJECT_H

#include "backend/sub/MainEvent.h"
#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct GmObject
{
  String name;
  int id;

  int spriteId;
  boolean solid;
  boolean visible;
  int depth;
  boolean persistent;
  int parentId;
  int maskId;
  // int polygonId;  // TODO: Uncomment if/when LGM supports polygons.

  MainEvent *mainEvents;
  int mainEventCount;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
