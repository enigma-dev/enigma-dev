package org.enigma.backend.other;

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Structure;

public class Extension extends Structure
	{
	public String name;
	public String path; //relative to enigma/ENIGMAsystem/SHELL/

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("name","path");
		}

	public static class ByReference extends Extension implements Structure.ByReference
		{
		}
	}
