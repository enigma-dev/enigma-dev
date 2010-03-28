/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import com.sun.jna.Structure;

public class BackgroundDef extends Structure
	{
	boolean visible;
	boolean foreground;
	int x;
	int y;
	boolean tileHoriz;
	boolean tileVert;
	int hSpeed;
	int vSpeed;
	boolean stretch;

	int backgroundId;
	}
