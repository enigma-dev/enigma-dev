/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */




#include "backend/JavaStruct.h"
#include "backend/util/Image.h"
//import org.lateralgm.file.iconio.ICOFile;

struct GameSettings
	{
	int gameId; // randomized in GmFile constructor
	boolean startFullscreen;
	boolean interpolate;
	boolean dontDrawBorder;
	boolean displayCursor;
	int scaling;
	boolean allowWindowResize;
	boolean alwaysOnTop;
	int colorOutsideRoom; //color
	boolean setResolution;
	byte colorDepth;
	byte resolution;
	byte frequency;
	boolean dontShowButtons;
	boolean useSynchronization;
	boolean disableScreensavers;
	boolean letF4SwitchFullscreen;
	boolean letF1ShowGameInfo;
	boolean letEscEndGame;
	boolean letF5SaveF6Load;
	boolean letF9Screenshot;
	boolean treatCloseAsEscape;
	byte gamePriority;
	boolean freezeOnLoseFocus;
	byte loadBarMode;
	//Image frontLoadBar;
	//Image backLoadBar;
	boolean showCustomLoadImage;
	//Image loadingImage;
	boolean imagePartiallyTransparent;
	int loadImageAlpha;
	boolean scaleProgressBar;
	boolean displayErrors;
	boolean writeToLog;
	boolean abortOnError;
	boolean treatUninitializedAs0;
	String author;
	String version;
	double lastChanged;
	String information;

	//FIXME: Includes information moved
	int includeFolder;
	boolean overwriteExisting;
	boolean removeAtGameEnd;

	int versionMajor;
	int versionMinor;
	int versionRelease;
	int versionBuild;
	String company;
	String product;
	String copyright;
	String description;

	String gameIcon; //filename
	};
