/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "../../backend/ideprint.h"

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../compile_common.h"

int compile_handle_templates(EnigmaStruct* es)
{
  map<string,string> values;
  GameSettings *gs = (GameSettings *)es;//es->gameSettings;
  values["gameid"]      = gs->gameId; // randomized in GmFile constructor
	values["fullscreen"]  = gs->startFullscreen;
	values["interpolate"] = gs->interpolate;
	values["drawborder"]  =!gs->dontDrawBorder;
	values["showcursor"]  = gs->displayCursor;
	values["scaling"]     = gs->scaling;
	values["allowresize"] = gs->allowWindowResize;
	values["alwaysontop"] = gs->alwaysOnTop;
	values["outsidecol"]  = gs->colorOutsideRoom; //color
	values["changeres"]   = gs->setResolution;
	values["colordepth"]  = gs->colorDepth;
	values["resolution"]  = gs->resolution;
	values["frequency"]   = gs->frequency;
	values["showbuttons"] =!gs->dontShowButtons;
	values["usevsync"]    = gs->useSynchronization;
	values["disablessvr"] = gs->disableScreensavers;
	values["f4fullscreen"]= gs->letF4SwitchFullscreen;
	values["f1gameinfo"]  = gs->letF1ShowGameInfo;
	values["escendgame"]  = gs->letEscEndGame;
	values["f56saveload"] = gs->letF5SaveF6Load;
	values["f9screenshot"]= gs->letF9Screenshot;
	values["closeasesc"]  = gs->treatCloseAsEscape;
	values["cpupriority"] = gs->gamePriority;
	values["focusfreeze"] = gs->freezeOnLoseFocus;
	values["loadbarmode"] = gs->loadBarMode;
	//values["loadbarfore"] = gs->frontLoadBar;
	//values["loadbarback"] = gs->backLoadBar;
	values["custloadimg"] = gs->showCustomLoadImage;
	//values["loadimage"]   = gs->loadingImage;
	values["imgtransp"]   = gs->imagePartiallyTransparent;
	values["loadalpha"]   = gs->loadImageAlpha;
	values["scalepbar"]   = gs->scaleProgressBar;
	values["showerrors"]  = gs->displayErrors;
	values["writetolog"]  = gs->writeToLog;
	values["abortonerr"]  = gs->abortOnError;
	values["zerouninit"]  = gs->treatUninitializedAs0;
	values["author"]      = gs->author;
	values["version"]     = gs->version;
	values["lastchanged"] = gs->lastChanged;
	values["information"] = gs->information;

	//FIXME: Includes information moved
	values["includes"]    = gs->includeFolder;
	values["overwrite"]   = gs->overwriteExisting;
	values["removeatend"] = gs->removeAtGameEnd;

	values["versionmajor"]   = gs->versionMajor;
	values["versionminor"]   = gs->versionMinor;
	values["versionrelease"] = gs->versionRelease;
	values["versionbuild"]   = gs->versionBuild;
	values["company"]        = gs->company;
	values["product"]        = gs->product;
	values["copyright"]      = gs->copyright;
	values["description"]    = gs->description;
	values["information"]    = gs->gameIcon;
	
  return 0;
}
