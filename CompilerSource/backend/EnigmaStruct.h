/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>, Josh Ventura
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_ENIGMASTRUCT_H
#define ENIGMA_ENIGMASTRUCT_H

#include "JavaStruct.h"
#include "resources/GameInformation.h"
#include "resources/GameSettings.h"
#include "resources/Sprite.h"
#include "resources/Sound.h"
#include "resources/Background.h"
#include "resources/Path.h"
#include "resources/Script.h"
#include "resources/Shader.h"
#include "resources/Font.h"
#include "resources/Timeline.h"
#include "resources/GmObject.h"
#include "resources/Room.h"
#include "resources/Polygon.h"

#include "other/Trigger.h"
#include "other/Constant.h"
#include "other/Include.h"
#include "other/Extension.h"

namespace deprecated {
namespace JavaStruct {

/// Structure once used for communicating game information between an IDE via
/// a shared-memory model. @deprecated Use `GameData` instead.
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
	Shader *shaders;
	int shaderCount;
	Font *fonts;
	int fontCount;
	Timeline *timelines;
	int timelineCount;
	GmObject *gmObjects;
	int gmObjectCount;
	Room *rooms;
	int roomCount;
	// TODO(Nabeel): Add this when you have Proto for Polygons
	// Polygon *polygons;
	// int polygonCount;

	Trigger *triggers;
	int triggerCount;
	Constant *constants;
	int constantCount;
	Include *includes;
	int includeCount;

	char **packages;

	int packageCount;

	Extension *extensions;
	int extensionCount;

	GameInformation gameInfo;
	GameSettings gameSettings;
	int lastInstanceId;
	int lastTileId;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
