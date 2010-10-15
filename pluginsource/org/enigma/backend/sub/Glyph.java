package org.enigma.backend.sub;

import java.nio.Buffer;

import com.sun.jna.Structure;

public class Glyph extends Structure
	{
	public int origin;
	public int baseline;
	public int advance;
	public int width;
	public int height;
	public Buffer raster; //size = width * height

	public static class ByReference extends Glyph implements Structure.ByReference
		{
		}
	}
