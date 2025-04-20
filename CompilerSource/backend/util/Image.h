/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_IMAGE_H
#define ENIGMA_IMAGE_H

struct Image
{
	int width;
	int height;
	char *data; //zlib compressed BGRA
	int dataSize;
};

#endif
