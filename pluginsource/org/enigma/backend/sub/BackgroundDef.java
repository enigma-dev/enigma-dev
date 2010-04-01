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
	public boolean visible;
	public boolean foreground;
	public int x;
	public int y;
	public boolean tileHoriz;
	public boolean tileVert;
	public int hSpeed;
	public int vSpeed;
	public boolean stretch;

	public int backgroundId;

	public static class ByReference extends BackgroundDef implements Structure.ByReference
		{
		}
	}
