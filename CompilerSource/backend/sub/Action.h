/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_ACTION__H
#define ENIGMA_ACTION__H

#include "backend/JavaStruct.h"

struct Action
{
	//LibAction libAction;
	boolean relative;
	boolean complement;
	int appliesToObjectId;

	//Argument[] arguments;
};

#endif
