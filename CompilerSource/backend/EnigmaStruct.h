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
#include "resources/gmObject.h"
#include "resources/Room.h"

#include "other/Trigger.h"
#include "other/Constant.h"
#include "other/Include.h"

struct EnigmaStruct
{
  int fileVersion;
  String filename;
  
  Sprite      *sprites;
  Sound       *sounds;
  Background  *backgrounds;
  Path        *paths;
  Script      *scripts;
  Font        *fonts;
  Timeline    *timelines;
  GmObject    *gmObjects;
  Room        *rooms;
  
	Trigger     *triggers;
	Constant    *constants;
	Include     *includes;
	String      *packages;

	//GameInformation gameInfo;
	//GameSettings gameSettings;
	int lastInstanceId;// = 100000;
	int lastTileId;// = 10000000;
};
