/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.sub;

import com.sun.jna.Structure;

public class View extends Structure
	{
	boolean visible;
	int viewX;
	int viewY;
	int viewW;
	int viewH;
	int portX;
	int portY;
	int portW;
	int portH;
	int borderH;
	int borderV;
	int speedH;
	int speedV;
	int objectId; //following object
	}
