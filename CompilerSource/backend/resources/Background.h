/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#include "../JavaStruct.h"
#include "../util/Image.h"

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
