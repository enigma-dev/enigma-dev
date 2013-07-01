/**
* @file gmxfilereader.h
* @brief Header file of the GMX file reader.
*
* Write a description about the file here...
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
*
* This file is a part of the ENIGMA Development Environment.
*
* ENIGMA is free software: you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the license or any later version.
*
* This application and its source code is distributed AS-IS, WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along
* with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef GMXFILEREADER_H_INCLUDED
#define GMXFILEREADER_H_INCLUDED

#include "backend/EnigmaStruct.h"

EnigmaStruct* loadGMX(char* path);

struct GMXFile
{
    EnigmaStruct* es;
    char* path;

    EnigmaStruct* load(char* path);
    void readSprites();
    void readSounds();
    void readBackgrounds();
    void readPaths();
    void readScripts();
    void readFonts();
    void readTimelines();
    void readObjects();
    void readRooms();
    void readGameSettings();
    void readGameInfo();
    void readGameExtensions();

};

#endif // GMXFILEREADER_H_INCLUDED
