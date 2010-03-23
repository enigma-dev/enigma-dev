/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import org.enigma.backend.resources.Background;
import org.enigma.backend.resources.Room;

import com.sun.jna.Structure;

public class Tile extends Structure
	{
	int bgX;
	int bgY;
	int roomX;
	int roomY;
	int width;
	int height;
	int depth;
	Background.ByReference background;
	int id;
	boolean locked;

	Room.ByReference room;
	}
