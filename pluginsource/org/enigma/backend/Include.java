/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend;

import com.sun.jna.Structure;

public class Include extends Structure
	{
	String filename = "";
	String filepath = "";
	boolean isOriginal;
	int size = 0;
	byte[] data = null;
	int export = 2;
	String exportFolder = "";
	boolean overwriteExisting = false;
	boolean freeMemAfterExport = true;
	boolean removeAtGameEnd = true;
	}
