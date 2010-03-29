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
	int snapX;
	
	int snapY;
	boolean isometric;
	int speed;
	boolean persistent;
	int backgroundColor;
	boolean drawBackgroundColor; //GM color
	String creationCode;
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

	BackgroundDef *backgroundDefs;
	View *views;
	Instance *instances;
	Tile *tiles;
};
