/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import com.sun.jna.Structure;

public class Font extends Structure
	{
	String name;
	int id;

	String FONT_NAME;
	int SIZE;
	boolean BOLD;
	boolean ITALIC;
	int RANGE_MIN;
	int RANGE_MAX;
	}
