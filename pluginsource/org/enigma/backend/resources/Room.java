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
	public String name;
	public int id;

	public String caption;
	public int width;
	public int height;

	// vvv may be useless vvv //
	public int snapX;
	public int snapY;
	public boolean isometric;
	// ^^^ may be useless ^^^ //

	public int speed;
	public boolean persistent;
	public int backgroundColor; //GM color
	public boolean drawBackgroundColor;
	public String creationCode;

	// vvv useless stuff vvv //
	public boolean rememberWindowSize;
	public int editorWidth;
	public int editorHeight;
	public boolean showGrid;
	public boolean showObjects;
	public boolean showTiles;
	public boolean showBackgrounds;
	public boolean showViews;
	public boolean deleteUnderlyingObjects;
	public boolean deleteUnderlyingTiles;
	public int currentTab;
	public int scrollBarX;
	public int scrollBarY;
	public boolean enableViews;
	// ^^^ useless stuff ^^^ //

	public BackgroundDef.ByReference backgroundDefs;
	public int backgroundDefCount;
	public View.ByReference views;
	public int viewCount;
	public Instance.ByReference instances;
	public int instanceCount;
	public Tile.ByReference tiles;
	public int tileCount;

	public static class ByReference extends Room implements Structure.ByReference
		{
		}
	}
