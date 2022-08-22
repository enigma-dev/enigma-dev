/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_BACKGROUNDDEF_H
#define ENIGMA_BACKGROUNDDEF_H

#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

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

}  // namespace JavaStruct
}  // namespace deprecated

#endif
