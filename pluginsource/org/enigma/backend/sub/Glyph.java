package org.enigma.backend.sub;

import java.nio.Buffer;

import com.sun.jna.Structure;

public class Glyph extends Structure
	{
	public double origin;
	public double baseline;
	public double advance;
	public int width;
	public int height;
	public Buffer raster; //size = width * height

	public static class ByReference extends Glyph implements Structure.ByReference
		{
		}
	}
