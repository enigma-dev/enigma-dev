/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import java.nio.Buffer;

import com.sun.jna.Structure;

public class Sound extends Structure
	{
	public String name;
	public int id;

	public int kind;
	public String fileType;
	public String fileName;
	public boolean chorus;
	public boolean echo;
	public boolean flanger;
	public boolean gargle;
	public boolean reverb;
	public double volume;
	public double pan;
	public boolean preload;

	public Buffer data;
	public int size;

	public static class ByReference extends Sound implements Structure.ByReference
		{
		}
	}
