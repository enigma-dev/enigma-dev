/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */
 
#ifndef _SUB_GLYPH_H
#define _SUB_GLYPH_H

struct Glyph
{
	int origin;
	int baseline;
	int advance;
	int width;
	int height;
	byte *data; //size = width * height
};

#endif
