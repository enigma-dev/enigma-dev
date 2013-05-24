package org.enigma.backend.util;

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Structure;

public class Polygon extends Structure
	{
	public Point.ByReference points;
	public int pointCount;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("points","pointCount");
		}

	public static class ByReference extends Polygon implements Structure.ByReference
		{
		}
	}
