/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef _PATH__H
#define _PATH__H

#include "backend/sub/PathPoint.h"
#include "backend/JavaStruct.h"

struct Path
{
	String name;
	int id;

	boolean smooth;
	boolean closed;
	int precision;
	
	//int backgroundRoomId; //This was used by LGM rather than ENIGMA; it doesn't need to be here.
	int snapX; //TODO: Find out if these are needed and decide on keeping them
	int snapY;

	PathPoint *points;
	int pointCount;
};

#endif
