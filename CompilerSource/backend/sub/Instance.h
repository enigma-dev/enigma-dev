/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef _INSTANCE__H
#define _INSTANCE__H

#include "backend/JavaStruct.h"

struct Instance
{
	int x;
	int y;
	int objectId;
	int id;
	String creationCode;
	boolean locked;
};

#endif
