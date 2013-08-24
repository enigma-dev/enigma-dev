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

	public String vcode;
	public String fcode;
	
	public String type;
	public boolean preload;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("name","id","vcode","fcode","type","preload");
		}

	public static class ByReference extends Shader implements Structure.ByReference
		{
		}
	}
