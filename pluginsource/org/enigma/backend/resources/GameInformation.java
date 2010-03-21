/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import com.sun.jna.Structure;

public class GameInformation extends Structure
	{
	int backgroundColor = 0xFFFFE100; //GM color
	boolean mimicGameWindow = false;
	String formCaption = "";
	int left = -1;
	int top = -1;
	int width = 600;
	int height = 400;
	boolean showBorder = true;
	boolean allowResize = true;
	boolean stayOnTop = false;
	boolean pauseGame = true;
	String gameInfoStr;
	}
