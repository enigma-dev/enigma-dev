/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import org.enigma.backend.sub.Image;

import com.sun.jna.Structure;

public class Background extends Structure
	{
	String name;
	int id;

	boolean transparent;
	boolean smoothEdges;
	boolean preload;
	boolean useAsTileset;
	int tileWidth;
	int tileHeight;
	int hOffset;
	int vOffset;
	int hSep;
	int vSep;

	Image backgroundImage;

	public class ByReference extends Background implements Structure.ByReference
		{
		}
	}
