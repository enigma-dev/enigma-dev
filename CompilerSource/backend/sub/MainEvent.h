/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_MAINEVENT_H
#define ENIGMA_MAINEVENT_H

#include "Event.h"
#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct MainEvent
{
	int id;
	Event *events;

	int eventCount;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
