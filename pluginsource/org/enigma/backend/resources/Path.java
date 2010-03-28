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

	boolean smooth;
	boolean closed;
	int precision;
	int backgroundRoomId;
	int snapX;
	int snapY;

	PathPoint[] points;
	int pointCount;
	}
