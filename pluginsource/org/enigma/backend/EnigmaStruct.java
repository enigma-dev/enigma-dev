/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend;

import org.enigma.backend.resources.Background;
import org.enigma.backend.resources.Font;
import org.enigma.backend.resources.GameInformation;
import org.enigma.backend.resources.GameSettings;
import org.enigma.backend.resources.GmObject;
import org.enigma.backend.resources.Path;
import org.enigma.backend.resources.Room;
import org.enigma.backend.resources.Script;
import org.enigma.backend.resources.Sound;
import org.enigma.backend.resources.Sprite;
import org.enigma.backend.resources.Timeline;

import com.sun.jna.Native;
import com.sun.jna.NativeLibrary;
import com.sun.jna.Structure;

public class EnigmaStruct extends Structure
	{
	/** One of 530, 600, 701, or 800 */
	public int fileVersion = 600;
	public String filename = null;

	public Sprite[] sprites;
	public int spriteCount;
	public Sound[] sounds;
	public int soundCount;
	public Background[] backgrounds;
	public int backgroundCount;
	public Path[] paths;
	public int pathCount;
	public Script[] scripts;
	public int scriptCount;
	public Font[] fonts;
	public int fontCount;
	public Timeline[] timelines;
	public int timelineCount;
	public GmObject[] gmObjects;
	public int gmObjectCount;
	public Room[] rooms;
	public int roomCount;

	public Trigger[] triggers;
	public int triggerCount;
	public Constant[] constants;
	public int constantCount;
	public Include[] includes;
	public int includeCount;
	public String[] packages;
	public int packageCount;

	public GameInformation gameInfo;
	public GameSettings gameSettings;
	public int lastInstanceId = 100000;
	public int lastTileId = 10000000;

	static
		{
		String lib = "exl";
		NativeLibrary.addSearchPath(lib,".");
		Native.register(lib);
		}

	public static void main(String[] args)
		{
		}
	}
