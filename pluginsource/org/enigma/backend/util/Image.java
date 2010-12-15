/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.util;

import java.nio.Buffer;

import com.sun.jna.Structure;

public class Image extends Structure
	{
	public int width;
	public int height;
	public Buffer data; //zlib compressed RGBA
	public int dataSize;

	public static class ByReference extends Image implements Structure.ByReference
		{
		}
	}
