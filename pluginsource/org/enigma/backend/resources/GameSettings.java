/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend.resources;

import org.enigma.backend.util.Image;
import org.lateralgm.file.iconio.ICOFile;

import com.sun.jna.Structure;

public class GameSettings extends Structure
	{
	int gameId; // randomized in GmFile constructor
	boolean startFullscreen = false;
	boolean interpolate = false;
	boolean dontDrawBorder = false;
	boolean displayCursor = true;
	int scaling = -1;
	boolean allowWindowResize = false;
	boolean alwaysOnTop = false;
	int colorOutsideRoom = 0x00000000; //color
	boolean setResolution = false;
	byte colorDepth;
	byte resolution;
	byte frequency;
	boolean dontShowButtons = false;
	boolean useSynchronization = false;
	boolean disableScreensavers = true;
	boolean letF4SwitchFullscreen = true;
	boolean letF1ShowGameInfo = true;
	boolean letEscEndGame = true;
	boolean letF5SaveF6Load = true;
	boolean letF9Screenshot = true;
	boolean treatCloseAsEscape = true;
	byte gamePriority;
	boolean freezeOnLoseFocus = false;
	byte loadBarMode;
	Image frontLoadBar = null;
	Image backLoadBar = null;
	boolean showCustomLoadImage = false;
	Image loadingImage = null;
	boolean imagePartiallyTransparent = false;
	int loadImageAlpha = 255;
	boolean scaleProgressBar = true;
	boolean displayErrors = true;
	boolean writeToLog = false;
	boolean abortOnError = false;
	boolean treatUninitializedAs0 = false;
	String author = "";
	String version = "100";
	double lastChanged;
	String information = "";

	//FIXME: Includes information moved
	int includeFolder;
	boolean overwriteExisting = false;
	boolean removeAtGameEnd = false;

	int versionMajor = 1;
	int versionMinor = 0;
	int versionRelease = 0;
	int versionBuild = 0;
	String company = "";
	String product = "";
	String copyright = "";
	String description = "";

	ICOFile gameIcon;
	}
