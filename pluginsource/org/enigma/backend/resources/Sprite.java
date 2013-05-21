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

import org.enigma.backend.sub.SubImage;
import org.enigma.backend.util.Polygon;

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

	public SubImage.ByReference subImages;
	public int subImageCount;

	public Polygon.ByReference maskShapes;
	public int maskShapeCount;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("name","id","transparent","shape","alphaTolerance","separateMask",
				"smoothEdges","preload","originX","originY","bbMode","bbLeft","bbRight","bbTop","bbBottom",
				"subImages","subImageCount","maskShapes","maskShapeCount");
		}

	public static class ByReference extends Sprite implements Structure.ByReference
		{
		}
	}
