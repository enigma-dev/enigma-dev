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

public class Action extends Structure
	{
	//	LibAction libAction;
	boolean relative;
	boolean not;
	int appliesToObjectId;

	//	Argument[] arguments;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("relative","not","appliesToObjectId");
		}
	}
