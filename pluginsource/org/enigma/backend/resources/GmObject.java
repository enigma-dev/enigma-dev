/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import org.enigma.backend.sub.Event;

import com.sun.jna.Structure;

public class GmObject extends Structure
	{
	String name;
	int id;

	Sprite.ByReference SPRITE;
	boolean SOLID;
	boolean VISIBLE;
	int DEPTH;
	boolean PERSISTENT;
	GmObject.ByReference PARENT;
	Sprite.ByReference MASK;

	Event[][] events;

	public static class ByReference extends GmObject implements Structure.ByReference
		{
		}
	}
