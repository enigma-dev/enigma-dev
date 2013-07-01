/**
* @file gmkfilereader.cpp
* @brief Source file of the GMK file reader.
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

#include "gmkfilereader.h"
#include "printcolors.h"

EnigmaStruct* loadGMK(char* path)
{
    GMKFile gmk;
    return gmk.load(path);
}

EnigmaStruct* GMKFile::load(char* path)
{
    es = new EnigmaStruct();
    es->filename = "examplegamefilename";
    file.open(path, ios::in|ios::binary);
    if (!file.is_open())
    {
        print_error("File does not exist");
        return 0;
    }

    int identifier;
    file.read((char*)&identifier, 4);
    if (identifier != 1234321)
    {
        print_error("Fuck you");
    }
    //throw new GmFormatException(f,Messages.format("GmFileReader.ERROR_INVALID",uri, //$NON-NLS-1$
    //identifier));
    int ver;
    file.read((char*)&ver, 4);
    cout << ver;

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
    file.close();

    return es;
}

void GMKFile::readSprites()
{

}

void GMKFile::readSounds()
{

}

void GMKFile::readBackgrounds()
{

}

void GMKFile::readPaths()
{

}

void GMKFile::readScripts()
{

}

void GMKFile::readFonts()
{

}
void GMKFile::readTimelines()
{

}

void GMKFile::readObjects()
{
    GmObject obj[10];
    obj[0] = GmObject();
    //obj[0].name="pissmonkey";
    obj[0].id = 0;

    //es->gmObjects = obj;
    //es->gmObjectCount = 1;
}

void GMKFile::readRooms()
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

void GMKFile::readGameSettings()
{
    es->gameSettings.alwaysOnTop = true;
    es->gameSettings.gameId = 03434534;
}

void GMKFile::readGameInfo()
{

}

void GMKFile::readGameExtensions()
{

}
