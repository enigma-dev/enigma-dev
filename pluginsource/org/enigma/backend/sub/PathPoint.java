/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import com.sun.jna.Structure;

public class PathPoint extends Structure
	{
	public int x;
	public int y;
	public int speed;

	public static class ByReference extends PathPoint implements Structure.ByReference
		{
		}
	}
