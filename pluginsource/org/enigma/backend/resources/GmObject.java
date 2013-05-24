/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import java.util.Arrays;
import java.util.List;

import org.enigma.backend.sub.MainEvent;

import com.sun.jna.Structure;

public class GmObject extends Structure
	{
	public String name;
	public int id;

	public int spriteId;
	public boolean solid;
	public boolean visible;
	public int depth;
	public boolean persistent;
	public int parentId;
	public int maskId; //sprite

	public MainEvent.ByReference mainEvents;
	public int mainEventCount;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("name","id","spriteId","solid","visible","depth","persistent","parentId",
				"maskId","mainEvents","mainEventCount");
		}

	public static class ByReference extends GmObject implements Structure.ByReference
		{
		}
	}
