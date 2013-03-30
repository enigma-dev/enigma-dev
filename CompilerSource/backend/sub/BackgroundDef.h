/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef _BACKGROUNDDEF__H
#define _BACKGROUNDDEF__H

#include "backend/JavaStruct.h"

struct BackgroundDef
{
	boolean visible;
	boolean foreground;
	int x;
	int y;
	boolean tileHoriz;
	boolean tileVert;
	int hSpeed;
	int vSpeed;
	boolean stretch;
	int backgroundId;
  /*  double alpha;
    int color;*/
};

#endif
