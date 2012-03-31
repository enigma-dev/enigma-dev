package org.enigma.backend.sub;

import org.enigma.backend.util.Image;

import com.sun.jna.Structure;

public class SubImage extends Structure
	{
	public Image image;

	public static class ByReference extends SubImage implements Structure.ByReference
		{
		}
	}
