/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */



#include "backend/JavaStruct.h"
#include "backend/util/Image.h"
#include "backend/util/Polygon.h"

struct SubImage
{
	Image image;
	Polygon *maskShapes;
	int maskShapeCount;
};
