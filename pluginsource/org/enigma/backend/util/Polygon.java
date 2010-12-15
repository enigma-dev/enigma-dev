package org.enigma.backend.util;

import com.sun.jna.Structure;

public class Polygon extends Structure
	{
	public Point.ByReference points;
	public int pointCount;

	public static class ByReference extends Polygon implements Structure.ByReference
		{
		}
	}
