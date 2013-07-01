/**
* @file egmfilereader.cpp
* @brief Source file of the EGM file reader.
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

#include <zip.h>

#include "egmfilereader.h"
#include "printcolors.h"

EnigmaStruct* loadEGM(char* path)
{
    EGMFile egm;
    return egm.load(path);
}

EnigmaStruct* EGMFile::load(char* path)
{
    es = new EnigmaStruct();
    es->filename = "examplegamefilename";

    readSprites();
    readSounds();
    readBackgrounds();
    readPaths();
    readScripts();
    readFonts();
    readTimelines();
    readObjects();
    readRooms();

    readGameSettings();
    readGameInfo();
    readGameExtensions();

    return es;
}

void EGMFile::readSprites()
{

}

void EGMFile::readSounds()
{

}

void EGMFile::readBackgrounds()
{

}

void EGMFile::readPaths()
{

}

void EGMFile::readScripts()
{

}

void EGMFile::readFonts()
{

}
void EGMFile::readTimelines()
{

}

void EGMFile::readObjects()
{
    GmObject obj[10];
    obj[0] = GmObject();
    //obj[0].name="pissmonkey";
    obj[0].id = 0;

    //es->gmObjects = obj;
    //es->gmObjectCount = 1;
}

void EGMFile::readRooms()
{
    Room rms[1];
    rms[0] = Room();
    rms[0].drawBackgroundColor = true;
    rms[0].width = 500;
    rms[0].height=500;
    rms[0].creationCode = "";
    rms[0].name = "exampleroom";
    rms[0].id = 0543;
    rms[0].speed = 30;
    rms[0].caption = "Example Game Room Caption";
    rms[0].instanceCount = 0;
    rms[0].backgroundColor = RGBA2DWORD(0, 149, 255, 255);

    es->rooms = rms;
    es->roomCount = 1;
}

void EGMFile::readGameSettings()
{
    es->gameSettings.alwaysOnTop = true;
    es->gameSettings.gameId = 03434534;
}

void EGMFile::readGameInfo()
{

}

void EGMFile::readGameExtensions()
{

}
