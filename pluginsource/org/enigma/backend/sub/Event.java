/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import org.enigma.backend.resources.GmObject;

import com.sun.jna.Structure;

public class Event extends Structure
	{
	int id;
	GmObject other;
	int mainId;

	Action[] actions;
	int actionCount;
	}
