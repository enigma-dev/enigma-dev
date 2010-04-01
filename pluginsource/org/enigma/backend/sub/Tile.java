/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import com.sun.jna.Structure;

public class Tile extends Structure
	{
	public int bgX;
	public int bgY;
	public int roomX;
	public int roomY;
	public int width;
	public int height;
	public int depth;
	public int backgroundId;
	public int id;
	public boolean locked;

	public static class ByReference extends Tile implements Structure.ByReference
		{
		}
	}
