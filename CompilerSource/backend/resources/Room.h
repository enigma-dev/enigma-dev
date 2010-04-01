/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */



#include "../sub/BackgroundDef.h"
#include "../sub/Instance.h"
#include "../sub/Tile.h"
#include "../sub/View.h"

#include "../JavaStruct.h"

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
	int backgroundColor;
	boolean drawBackgroundColor; //GM color
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
	boolean enableViews;
	// ^^^ useless stuff ^^^ //

	BackgroundDef *backgroundDefs;
	int backgroundDefCount;
	View *views;
	int viewCount;
	Instance *instances;
	int instanceCount;
	Tile *tiles;
	int tileCount;
};
