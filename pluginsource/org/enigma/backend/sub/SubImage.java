package org.enigma.backend.sub;

import org.enigma.backend.util.Image;
import org.enigma.backend.util.Polygon;

import com.sun.jna.Structure;

public class SubImage extends Structure
	{
	public Image image;
	public Polygon.ByReference maskShapes;
	public int maskShapeCount;

	public static class ByReference extends SubImage implements Structure.ByReference
		{
		}
	}
