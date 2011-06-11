package org.enigma.backend.other;

import com.sun.jna.Structure;

public class Extension extends Structure
	{
	public String name;
	public String path; //relative to enigma/ENIGMAsystem/SHELL/

	public static class ByReference extends Extension implements Structure.ByReference
		{
		}
	}
