/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import com.sun.jna.Structure;

public class GameSettings extends Structure
	{
	public int gameId; // randomized in GmFile constructor
	public boolean startFullscreen = false;
	public boolean interpolate = false;
	public boolean dontDrawBorder = false;
	public boolean displayCursor = true;
	public int scaling = -1;
	public boolean allowWindowResize = false;
	public boolean alwaysOnTop = false;
	public int colorOutsideRoom = 0x00000000; //color
	public boolean setResolution = false;
	public byte colorDepth;
	public byte resolution;
	public byte frequency;
	public boolean dontShowButtons = false;
	public boolean useSynchronization = false;
	public boolean disableScreensavers = true;
	public boolean letF4SwitchFullscreen = true;
	public boolean letF1ShowGameInfo = true;
	public boolean letEscEndGame = true;
	public boolean letF5SaveF6Load = true;
	public boolean letF9Screenshot = true;
	public boolean treatCloseAsEscape = true;
	public byte gamePriority;
	public boolean freezeOnLoseFocus = false;
	public byte loadBarMode;
	//	public Image frontLoadBar = null;
	//	public Image backLoadBar = null;
	public boolean showCustomLoadImage = false;
	//	public Image loadingImage = null;
	public boolean imagePartiallyTransparent = false;
	public int loadImageAlpha = 255;
	public boolean scaleProgressBar = true;
	public boolean displayErrors = true;
	public boolean writeToLog = false;
	public boolean abortOnError = false;
	public boolean treatUninitializedAs0 = false;
	public String author;
	public String version;
	public double lastChanged;
	public String information;

	//FIXME: Includes information moved
	public int includeFolder;
	public boolean overwriteExisting = false;
	public boolean removeAtGameEnd = false;

	public int versionMajor = 1;
	public int versionMinor = 0;
	public int versionRelease = 0;
	public int versionBuild = 0;
	public String company;
	public String product;
	public String copyright;
	public String description;

	public String gameIcon;
	}
