/**
* @file gmkfilereader.h
* @brief Header file of the GMK file reader.
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

#ifndef GMKFILEREADER_H_INCLUDED
#define GMKFILEREADER_H_INCLUDED

#include <iostream>
#include <fstream>
using namespace std;

#include "backend/EnigmaStruct.h"

EnigmaStruct* loadGMK(char* path);

struct GMKFile
{
    EnigmaStruct* es;
    char* path;
    ifstream file;

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

#endif // GMKFILEREADER_H_INCLUDED
