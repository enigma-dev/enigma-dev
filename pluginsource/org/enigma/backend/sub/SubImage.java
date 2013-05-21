package org.enigma.backend.sub;

import java.util.Arrays;
import java.util.List;

import org.enigma.backend.util.Image;

import com.sun.jna.Structure;

public class SubImage extends Structure
	{
	public Image image;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("image");
		}

	public static class ByReference extends SubImage implements Structure.ByReference
		{
		}
	}
