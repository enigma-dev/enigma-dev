/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */


#include "../JavaStruct.h"

struct Sound
{
	String name;
	int id;

	int kind;
	int fileType;
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
};
