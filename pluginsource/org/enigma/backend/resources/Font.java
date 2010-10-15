/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import org.enigma.backend.sub.Glyph;

import com.sun.jna.Structure;

public class Font extends Structure
	{
	public String name;
	public int id;

	public String fontName;
	public int size;
	public boolean bold;
	public boolean italic;
	public int rangeMin;
	public int rangeMax;

	public Glyph.ByReference glyphs; //count = rangeMax - rangeMin

	public static class ByReference extends Font implements Structure.ByReference
		{
		}
	}
