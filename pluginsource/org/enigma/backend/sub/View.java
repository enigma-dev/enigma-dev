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

public class View extends Structure
	{
	boolean VISIBLE;
	int VIEW_X;
	int VIEW_Y;
	int VIEW_W;
	int VIEW_H;
	int PORT_X;
	int PORT_Y;
	int PORT_W;
	int PORT_H;
	int BORDER_H;
	int BORDER_V;
	int SPEED_H;
	int SPEED_V;
	GmObject.ByReference OBJECT;
	}
