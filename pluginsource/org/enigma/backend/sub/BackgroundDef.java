/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import org.enigma.backend.resources.Background;

import com.sun.jna.Structure;

public class BackgroundDef extends Structure
	{
	boolean VISIBLE;
	boolean FOREGROUND;
	boolean BACKGROUND;
	int X;
	int Y;
	boolean TILE_HORIZ;
	boolean TILE_VERT;
	int H_SPEED;
	int V_SPEED;
	boolean STRETCH;

	Background.ByReference background;
	}
