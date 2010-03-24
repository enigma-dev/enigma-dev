/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import org.enigma.backend.sub.BackgroundDef;
import org.enigma.backend.sub.Instance;
import org.enigma.backend.sub.Tile;
import org.enigma.backend.sub.View;

import com.sun.jna.Structure;

public class Room extends Structure
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

	BackgroundDef[] backgroundDefs;
	int backgroundDefCount;
	View[] views;
	int viewCount;
	Instance[] instances;
	int instanceCount;
	Tile[] tiles;
	int tileCount;

	public static class ByReference extends Room implements Structure.ByReference
		{
		}
	}
