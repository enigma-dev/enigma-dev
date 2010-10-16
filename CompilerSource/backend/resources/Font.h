/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */



#include "../JavaStruct.h"
#include "../sub/Glyph.h"

struct Font
{
	String name;
	int id;
  
	String fontName;
	int size;
	boolean bold;
	boolean italic;
	int rangeMin;
	int rangeMax;

	Glyph *glyphs; //count = rangeMax - rangeMin
};
