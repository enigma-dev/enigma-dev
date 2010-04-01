/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import com.sun.jna.Structure;

public class Event extends Structure
	{
	public int id;
	public int otherObjectId; //collision, etc
	public int mainId;

	public String code;

	public static class ByReference extends Event implements Structure.ByReference
		{
		}
	}
