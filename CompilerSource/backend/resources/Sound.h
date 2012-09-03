/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef _SOUND__H
#define _SOUND__H

#include "backend/JavaStruct.h"

struct Sound
{
	String name;
	int id;

	int kind;
	String fileType;
	String fileName;
	boolean chorus;
	boolean echo;
	boolean flanger;
	boolean gargle;
	boolean reverb;
	double volume;
	double pan;
	boolean preload;

	byte *data;
	int size;
};

#endif
