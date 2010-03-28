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

public class Sprite extends Structure
	{
	public String name;
	public int id;

	public boolean transparent;
	public int shape; //0*=Precise, 1=Rectangle,  2=Disk, 3=Diamond
	public int alphaTolerance;
	public boolean separateMask;
	public boolean smoothEdges;
	public boolean preload;
	public int originX;
	public int originY;
	public int bbMode; //0*=Automatic, 1=Full image, 2=Manual
	public int bbLeft;
	public int bbRight;
	public int bbTop;
	public int bbBottom;

	public Image[] subImages;
	public int subImageCount;

	public static class ByReference extends Sprite implements Structure.ByReference
		{
		}
	}
