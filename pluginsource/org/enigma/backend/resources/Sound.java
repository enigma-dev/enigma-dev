/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import com.sun.jna.Structure;

public class Sound extends Structure
	{
	String name;
	int id;

	int KIND;
	int FILE_TYPE;
	String FILE_NAME;
	boolean CHORUS;
	boolean ECHO;
	boolean FLANGER;
	boolean GARGLE;
	boolean REVERB;
	double VOLUME;
	double PAN;
	boolean PRELOAD;

	byte[] data;
	}
