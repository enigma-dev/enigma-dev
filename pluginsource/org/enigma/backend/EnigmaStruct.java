/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend;

import org.enigma.backend.other.Constant;
import org.enigma.backend.other.Extension;
import org.enigma.backend.other.Include;
import org.enigma.backend.other.Trigger;
import org.enigma.backend.resources.Background;
import org.enigma.backend.resources.Font;
import org.enigma.backend.resources.GmObject;
import org.enigma.backend.resources.Path;
import org.enigma.backend.resources.Room;
import org.enigma.backend.resources.Script;
import org.enigma.backend.resources.Sound;
import org.enigma.backend.resources.Sprite;
import org.enigma.backend.resources.Timeline;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;

public class EnigmaStruct extends Structure
	{
	/** One of 530, 600, 701, or 800 */
	public int fileVersion = 600;
	public String filename = null;

	public Sprite.ByReference sprites;
	public int spriteCount;
	public Sound.ByReference sounds;
	public int soundCount;
	public Background.ByReference backgrounds;
	public int backgroundCount;
	public Path.ByReference paths;
	public int pathCount;
	public Script.ByReference scripts;
	public int scriptCount;
	public Font.ByReference fonts;
	public int fontCount;
	public Timeline.ByReference timelines;
	public int timelineCount;
	public GmObject.ByReference gmObjects;
	public int gmObjectCount;
	public Room.ByReference rooms;
	public int roomCount;

	public Trigger.ByReference triggers;
	public int triggerCount;
	public Constant.ByReference constants;
	public int constantCount;
	public Include.ByReference includes;
	public int includeCount;
	public Pointer packages; //StringArray
	public int packageCount;

	public Extension.ByReference extensions;
	public int extensionCount;

	//public GameInformation gameInfo;
	//public GameSettings gameSettings;
	public int lastInstanceId = 100000;
	public int lastTileId = 10000000;

	}
