/*
 * Copyright (C) 2013 Robert B. Colton
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Structure;

public class Shader extends Structure
	{
	public String name;
	public int id;

	public String vertex;
	public String fragment;
	
	public String type;
	public boolean precompile;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("name","id","vertex","fragment","type","precompile");
		}

	public static class ByReference extends Shader implements Structure.ByReference
		{
		}
	}
