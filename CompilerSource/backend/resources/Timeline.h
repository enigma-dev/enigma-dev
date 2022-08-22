/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_TIMELINE_H
#define ENIGMA_TIMELINE_H

#include "backend/sub/Moment.h"
#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct Timeline
{
	String name;
	int id;

	Moment *moments;
	int momentCount;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
