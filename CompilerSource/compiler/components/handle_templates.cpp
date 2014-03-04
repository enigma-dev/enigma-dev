/**
  @file  handle_templates.cpp
  @brief Implements the method in the C++ plugin that reads in template source files
         and parses them into compilable sources, in a manner much like CMake.
  
  @section License
    Copyright (C) 2011-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "backend/ideprint.h"

using namespace std;

#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"

/// This is the function that never got to be. It is meant to allow using templates
/// to create source/resource files using properties from the game. The function is
/// **supposed** to pack a bunch of properties into a map by name, then traverse an
/// assortment of "template" files, filling in fields to generate the new resource.
/// In fact, it only gets as far as populating the map.
// TODO: Finish.
int lang_CPP::compile_handle_templates(compile_context &ctex)
{
  map<string,string> values;
  GameSettings &gs = ctex.es->gameSettings;
  values["gameid"]      = gs.gameId;
	values["fullscreen"]  = gs.startFullscreen;
	values["interpolate"] = gs.interpolate;
	values["forcesoftwarevertexprocessing"] = gs.forceSoftwareVertexProcessing;
	values["drawborder"]  =!gs.dontDrawBorder;
	values["showcursor"]  = gs.displayCursor;
	values["scaling"]     = gs.scaling;
	values["allowresize"] = gs.allowWindowResize;
	values["alwaysontop"] = gs.alwaysOnTop;
	values["outsidecol"]  = gs.colorOutsideRoom;
	values["changeres"]   = gs.setResolution;
	values["colordepth"]  = gs.colorDepth;
	values["resolution"]  = gs.resolution;
	values["frequency"]   = gs.frequency;
	values["showbuttons"] =!gs.dontShowButtons;
	values["usevsync"]    = gs.useSynchronization;
	values["disablessvr"] = gs.disableScreensavers;
	values["f4fullscreen"]= gs.letF4SwitchFullscreen;
	values["f1gameinfo"]  = gs.letF1ShowGameInfo;
	values["escendgame"]  = gs.letEscEndGame;
	values["f56saveload"] = gs.letF5SaveF6Load;
	values["f9screenshot"]= gs.letF9Screenshot;
	values["closeasesc"]  = gs.treatCloseAsEscape;
	values["cpupriority"] = gs.gamePriority;
	values["focusfreeze"] = gs.freezeOnLoseFocus;
	values["loadbarmode"] = gs.loadBarMode;
//values["loadbarfore"] = gs.frontLoadBar;
//values["loadbarback"] = gs.backLoadBar;
	values["custloadimg"] = gs.showCustomLoadImage;
//values["loadimage"]   = gs.loadingImage;
	values["imgtransp"]   = gs.imagePartiallyTransparent;
	values["loadalpha"]   = gs.loadImageAlpha;
	values["scalepbar"]   = gs.scaleProgressBar;
	values["showerrors"]  = gs.displayErrors;
	values["writetolog"]  = gs.writeToLog;
	values["abortonerr"]  = gs.abortOnError;
	values["zerouninit"]  = gs.treatUninitializedAs0;
	values["author"]      = gs.author;
	values["version"]     = gs.version;
	values["lastchanged"] = gs.lastChanged;
	values["information"] = gs.information;

	//FIXME: Includes information moved
	values["includes"]    = gs.includeFolder;
	values["overwrite"]   = gs.overwriteExisting;
	values["removeatend"] = gs.removeAtGameEnd;

	values["versionmajor"]   = gs.versionMajor;
	values["versionminor"]   = gs.versionMinor;
	values["versionrelease"] = gs.versionRelease;
	values["versionbuild"]   = gs.versionBuild;
	values["company"]        = gs.company;
	values["product"]        = gs.product;
	values["copyright"]      = gs.copyright;
	values["description"]    = gs.description;
	values["information"]    = gs.gameIcon;
	
	// TODO: Iterate template files here, generating resource files using the values[] map.
	
  return 0;
}
