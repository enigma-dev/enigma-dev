/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend;

import org.lateralgm.resources.sub.Event;

import com.sun.jna.Structure;

public class Trigger extends Structure
	{
	String name = "";
	String condition = "";
	int checkStep = Event.EV_STEP_NORMAL;
	String constant = "";

	public static class ByReference extends Trigger implements Structure.ByReference
		{
		}
	}
