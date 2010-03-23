/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import org.enigma.backend.resources.GmObject;
import org.enigma.backend.resources.Room;

import com.sun.jna.Structure;

public class Instance extends Structure
	{
	int x;
	int y;
	GmObject.ByReference object;
	int id;
	String creationCode;
	boolean locked;

	Room.ByReference room;
	}
