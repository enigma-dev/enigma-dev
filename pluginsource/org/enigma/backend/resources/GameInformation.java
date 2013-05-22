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

import com.sun.jna.Structure;

public class GameInformation extends Structure
	{
	int backgroundColor; //default GM color 0xFFFFE100
	boolean mimicGameWindow;
	String formCaption;
	int left;
	int top;
	int width;
	int height;
	boolean showBorder;
	boolean allowResize;
	boolean stayOnTop;
	boolean pauseGame;
	String gameInfoStr;

	@Override
	protected List<String> getFieldOrder()
		{
		return Arrays.asList("backgroundColor","mimicGameWindow","formCaption","left","top","width",
				"height","showBorder","allowResize","stayOnTop","pauseGame","gameInfoStr");
		}
	}
