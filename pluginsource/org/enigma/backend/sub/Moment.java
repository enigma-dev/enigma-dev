/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Structure;

public class Moment extends Structure
	{
	public int stepNo;
	public String code;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("stepNo","code");
		}

	public static class ByReference extends Moment implements Structure.ByReference
		{
		}
	}
