/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */



#include "backend/sub/BackgroundDef.h"
#include "backend/sub/Instance.h"
#include "backend/sub/Tile.h"
#include "backend/sub/View.h"

#include "backend/JavaStruct.h"

struct Room
{
	String name;
	int id;

	String caption;
	int width;
	int height;

	// vvv may be useless vvv //
	int snapX;
	int snapY;
	boolean isometric;
	// ^^^ may be useless ^^^ //

	int speed;
	boolean persistent;
	int backgroundColor; //RGBA
	boolean drawBackgroundColor;
	String creationCode;

	// vvv useless stuff vvv //
	boolean rememberWindowSize;
	int editorWidth;
	int editorHeight;
	boolean showGrid;
	boolean showObjects;
	boolean showTiles;
	boolean showBackgrounds;
	boolean showViews;
	boolean deleteUnderlyingObjects;
	boolean deleteUnderlyingTiles;
	int currentTab;
	int scrollBarX;
	int scrollBarY;
	// ^^^ useless stuff ^^^ //
	
	// Ism thought this one was useless:
	boolean enableViews;

	BackgroundDef *backgroundDefs;
	int backgroundDefCount;
	View *views;
	int viewCount;
	Instance *instances;
	int instanceCount;
	Tile *tiles;
	int tileCount;
};
