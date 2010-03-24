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
	String name;
	int id;

	boolean transparent;
	int shape; //0*=Precise, 1=Rectangle,  2=Disk, 3=Diamond
	int alphaTolerance;
	boolean separateMask;
	boolean smoothEdges;
	boolean preload;
	int originX;
	int originY;
	int bbMode; //0*=Automatic, 1=Full image, 2=Manual
	int bbLeft;
	int bbRight;
	int bbTop;
	int bbBottom;

	Image[] subImages;
	int subImageCount;

	public static class ByReference extends Sprite implements Structure.ByReference
		{
		}
	}
