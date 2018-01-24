/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_MAINEVENT__H
#define ENIGMA_MAINEVENT__H

#include "Event.h"
#include "backend/JavaStruct.h"

struct MainEvent
{
	int id;
	Event *events;

	int eventCount;
};

#endif
