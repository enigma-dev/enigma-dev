/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import com.sun.jna.Structure;

public class Sound extends Structure
	{
	String name;
	int id;

	int kind;
	int fileType;
	String fileName;
	boolean chorus;
	boolean echo;
	boolean flanger;
	boolean gargle;
	boolean reverb;
	double volume;
	double pan;
	boolean preload;

	byte[] data;
	int size;

	public static class ByReference extends Sound implements Structure.ByReference
		{
		}
	}
