/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#include "JavaStruct.h"
#include "resources/Sprite.h"
#include "resources/Sound.h"
#include "resources/Background.h"
#include "resources/Path.h"
#include "resources/Script.h"
#include "resources/Font.h"
#include "resources/Timeline.h"
#include "resources/GmObject.h"
#include "resources/Room.h"

#include "other/Trigger.h"
#include "other/Constant.h"
#include "other/Include.h"

struct EnigmaStruct
{
  int fileVersion;
	String filename;

	Sprite *sprites;
	int spriteCount;
	Sound *sounds;
	int soundCount;
	Background *backgrounds;
	int backgroundCount;
	Path *paths;
	int pathCount;
	Script *scripts;
	int scriptCount;
	Font *fonts;
	int fontCount;
	Timeline *timelines;
	int timelineCount;
	GmObject *gmObjects;
	int gmObjectCount;
	Room *rooms;
	int roomCount;

	Trigger *triggers;
	int triggerCount;
	Constant *constants;
	int constantCount;
	Include *includes;
	int includeCount;
	char *packages[];
	int packageCount;

	//GameInformation gameInfo;
	//GameSettings gameSettings;
	int lastInstanceId;
	int lastTileId;
};

#undef String
#undef boolean
#undef byte
