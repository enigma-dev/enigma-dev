/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */



#include "../JavaStruct.h"

struct Event
{
	int id;
	int otherObjectId; //FIXME: This started as a GmObject, not even a reference. No idea what it does; revised to int.
	int mainId;

	String code;
};
