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

	String fontName;
	int size;
	boolean bold;
	boolean italic;
	int rangeMin;
	int rangeMax;

	public static class ByReference extends Font implements Structure.ByReference
		{
		}
	}
