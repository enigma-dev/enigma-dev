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
	public boolean visible;
	public int viewX;
	public int viewY;
	public int viewW;
	public int viewH;
	public int portX;
	public int portY;
	public int portW;
	public int portH;
	public int borderH;
	public int borderV;
	public int speedH;
	public int speedV;
	public int objectId; //following object

	public static class ByReference extends View implements Structure.ByReference
		{
		}
	}
