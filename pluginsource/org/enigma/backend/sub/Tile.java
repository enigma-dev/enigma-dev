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
	int BG_X;
	int BG_Y;
	int ROOM_X;
	int ROOM_Y;
	int WIDTH;
	int HEIGHT;
	int DEPTH;
	Background.ByReference BACKGROUND;
	int ID;
	boolean LOCKED;
	
	Room.ByReference room;
	}
