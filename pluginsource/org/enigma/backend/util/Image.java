/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.util;

import java.nio.Buffer;
import java.util.Arrays;
import java.util.List;

import com.sun.jna.Structure;

public class Image extends Structure
	{
	public int width;
	public int height;
	public Buffer data; //zlib compressed RGBA
	public int dataSize;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("width","height","data","dataSize");
		}

	public static class ByReference extends Image implements Structure.ByReference
		{
		}
	}
