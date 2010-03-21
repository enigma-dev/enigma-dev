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

	String CAPTION;
	int WIDTH;
	int HEIGHT;
	int SNAP_X;
	int SNAP_Y;
	boolean ISOMETRIC;
	int SPEED;
	boolean PERSISTENT;
	int BACKGROUND_COLOR; //GM color
	boolean DRAW_BACKGROUND_COLOR;
	String CREATION_CODE;
	boolean REMEMBER_WINDOW_SIZE;
	int EDITOR_WIDTH;
	int EDITOR_HEIGHT;
	boolean SHOW_GRID;
	boolean SHOW_OBJECTS;
	boolean SHOW_TILES;
	boolean SHOW_BACKGROUNDS;
	boolean SHOW_FOREGROUNDS;
	boolean SHOW_VIEWS;
	boolean DELETE_UNDERLYING_OBJECTS;
	boolean DELETE_UNDERLYING_TILES;
	int CURRENT_TAB;
	int SCROLL_BAR_X;
	int SCROLL_BAR_Y;
	boolean ENABLE_VIEWS;

	BackgroundDef[] backgroundDefs;
	View[] views;
	Instance[] instances;
	Tile[] tiles;

	public static class ByReference extends Room implements Structure.ByReference
		{
		}
	}
