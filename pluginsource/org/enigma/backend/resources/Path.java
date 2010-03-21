/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import org.enigma.backend.sub.PathPoint;

import com.sun.jna.Structure;

public class Path extends Structure
	{
	String name;
	int id;

	boolean SMOOTH;
	boolean CLOSED;
	int PRECISION;
	Room.ByReference BACKGROUND_ROOM;
	int SNAP_X;
	int SNAP_Y;

	PathPoint[] points;
	}
