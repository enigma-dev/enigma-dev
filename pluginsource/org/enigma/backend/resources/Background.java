/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import org.enigma.backend.util.Image;

import com.sun.jna.Structure;

public class Background extends Structure
	{
	public String name;
	public int id;

	public boolean transparent;
	public boolean smoothEdges;
	public boolean preload;
	public boolean useAsTileset;
	public int tileWidth;
	public int tileHeight;
	public int hOffset;
	public int vOffset;
	public int hSep;
	public int vSep;

	public Image backgroundImage;

	public static class ByReference extends Background implements Structure.ByReference
		{
		}
	}
