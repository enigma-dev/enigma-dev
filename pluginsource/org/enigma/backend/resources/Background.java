/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import java.util.Arrays;
import java.util.List;

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

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("name","id","transparent","smoothEdges","preload","useAsTileset",
				"tileWidth","tileHeight","hOffset","vOffset","hSep","vSep","backgroundImage");
		}

	public static class ByReference extends Background implements Structure.ByReference
		{
		}
	}
