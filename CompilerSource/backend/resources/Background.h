/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_BACKGROUND_H
#define ENIGMA_BACKGROUND_H

#include "backend/JavaStruct.h"
#include "backend/util/Image.h"

namespace deprecated {
namespace JavaStruct {

struct Background
{
  String name;
  int id;

  boolean transparent;
  boolean smoothEdges;
  boolean preload;
  boolean useAsTileset;
  int tileWidth;
  int tileHeight;
  int hOffset;
  int vOffset;
  int hSep;
  int vSep;

  Image backgroundImage;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
