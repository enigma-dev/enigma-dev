/**
* @file main.cpp
* @brief Source file implementing the main entry build of the ENIGMA make program.
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
* This file is part of ENIGMA.
*
* ENIGMA is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the License, or (at your option) any later version.
*
* ENIGMA is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* ENIGMA. If not, see <http://www.gnu.org/licenses/>.
**/

#include <windows.h>

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using std::vector;
#include "library.h"

#include "rapidxml-1.13/rapidxml.hpp"

//#include "gmk.hpp"

using namespace std;
using namespace rapidxml;

unsigned long RGBA2DWORD(int iR, int iG, int iB, int iA)
{
  return (((((iR << 8) + iG) << 8) + iB) << 8) + iA;
}

inline string string_replace_all(string str,string substr,string nstr)
{
  size_t pos=0;
  while ((pos=str.find(substr,pos)) != string::npos)
  {
    str.replace(pos,substr.length(),nstr);
    pos+=nstr.length();
  }
  return str;
}

void buildtestproject(const char* output) {
    EnigmaStruct* es = new EnigmaStruct();
    es->gameSettings.gameIcon = "../../Resources/joshcontroller.ico";
    es->gameSettings.letEscEndGame = true;
    es->gameSettings.treatCloseAsEscape = true;
    es->gameSettings.alwaysOnTop = true;
    es->gameSettings.gameId = 03434534;
    es->gameSettings.company = "";
    es->gameSettings.description = "";
    es->gameSettings.version = "";
    es->gameSettings.product = "";
    es->gameSettings.version = "";
    es->gameSettings.copyright = "";
    es->gameInfo.gameInfoStr = "";
    es->gameInfo.formCaption = "";
    es->filename = "exampleproject";
    Room rms[1];
    rms[0] = Room();
    rms[0].drawBackgroundColor = true;
    rms[0].width = 500;
    rms[0].height = 500;
    rms[0].creationCode = "";
    rms[0].name = "exampleroom";
    rms[0].id = 0;
    rms[0].speed = 30;
    rms[0].caption = "Example Game Room Caption";
    rms[0].instanceCount = 0;
    rms[0].backgroundColor = RGBA2DWORD(3, 149, 255, 255);
    
    GmObject obj[10];
    obj[0] = GmObject();
    //obj[0].name="pissmonkey";
    obj[0].id = 0;
    // Sprite spr[0];
    // Font fnt[0];
    // Timeline tln[0];
    // Script scr[0];
    // Path pth[0];
    // Background bgd[0];

    es->rooms = rms;
    es->roomCount = 1;
    //es->gmObjects = obj;
    //es->gmObjectCount = 1;
    // es->scripts = scr;
    // es->fonts = fnt;
    // es->sprites = spr;
    // es->timelines = tln;
    // es->backgrounds = bgd;
    cout << compileEGMf(es, output, emode_run) << endl;
}

vector<Room*> rooms;
vector<GmObject*> objects;
vector<Script*> scripts;
vector<Sprite*> sprites;
vector<Shader*> shaders;
vector<Font*> fonts;
vector<Timeline*> timelines;
vector<Background*> backgrounds;
vector<Path*> paths;

Room* readGMXRoom(const char* path) {
    string filepath = path;
    string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
    MessageBox(NULL, name.c_str(), "wtf", MB_OK);
    
    
  Room* rm = new Room();
  rm->drawBackgroundColor = true;
  rm->width = 500;
  rm->height = 500;
  rm->creationCode = "";
  rm->name = name.c_str();
  rm->id = rooms.size();
  
  stringstream ss;
  ss << rm->id;
  MessageBox(NULL, ss.str().c_str(), "wtf", MB_OK);
  
  rm->speed = 30;
  rm->caption = "Example Game Room Caption";
  rm->instanceCount = 0;
  rm->backgroundColor = RGBA2DWORD(3, 149, 255, 255);
  return rm;
}

Sprite* readGMXSprite(const char* path) {

}

GmObject* readGMXObject(const char* path) {

}

Timeline* readGMXTimeline(const char* path) {

}

Sound* readGMXSound(const char* path) {

}

Font* readGMXFont(const char* path) {

}

Script* readGMXScript(const char* path) {

}

Shader* readGMXShader(const char* path) {

}

Path* readGMXPath(const char* path) {

}

Background* readGMXBackground(const char* path) {

}

void buildgmx(const char* input, const char* output) {
    xml_document<> doc;    // character type defaults to char
    std::ifstream filestream(input);
    std::stringstream buffer;
    buffer << filestream.rdbuf();
    filestream.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);    // 0 means default parse flags
    xml_node<> *pRoot = doc.first_node();
    
    string filepath = input;
    string folder = filepath.substr(0, filepath.find_last_of('/') + 1);
    MessageBox(NULL, folder.c_str(), "wtf", MB_OK);

    EnigmaStruct* es = new EnigmaStruct();
    es->gameSettings.gameIcon = "../../Resources/joshcontroller.ico";
    es->gameSettings.letEscEndGame = true;
    es->gameSettings.treatCloseAsEscape = true;
    es->gameSettings.alwaysOnTop = true;
    es->gameSettings.gameId = 03434534;
    es->gameSettings.company = "";
    es->gameSettings.description = "";
    es->gameSettings.version = "";
    es->gameSettings.product = "";
    es->gameSettings.version = "";
    es->gameSettings.copyright = "";
    es->gameInfo.gameInfoStr = "";
    es->gameInfo.formCaption = "";
    es->filename = "exampleproject";
    
    for (xml_node<> *node = pRoot->first_node(); node; node = node->next_sibling())
    {
      if (strcmp(node->name(), "sprites") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          //sprites.push_back(readGMXSprite());
        }
      } else if (strcmp(node->name(), "sounds") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          //sounds.push_back(readGMXSound());
        }
      } else if (strcmp(node->name(), "backgrounds") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          //backgrounds.push_back(readGMXBackground());
        }
      } else if (strcmp(node->name(), "objects") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          //objects.push_back(readGMXObject());
        }
      } else if (strcmp(node->name(), "timelines") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          //timelines.push_back(readGMXTimeline());
        }
      } else if (strcmp(node->name(), "paths") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
         // paths.push_back(readGMXPath());
        }
      } else if (strcmp(node->name(), "scripts") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          //scripts.push_back(readGMXScript());
        }
      } else if (strcmp(node->name(), "shaders") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          //shaders.push_back(readGMXShader());
        }
      } else if (strcmp(node->name(), "fonts") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
         // fonts.push_back(readGMXFont());
        }
      } else if (strcmp(node->name(), "rooms") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          rooms.push_back(readGMXRoom( (folder + string_replace_all(resnode->value(), "\\", "/")).c_str() ));
        }
      }
    }
    
    MessageBox(NULL, rooms[0]->name, "wtf donkey", MB_OK);
    es->rooms = rooms.size() > 0 ? rooms[0] : NULL;
    MessageBox(NULL, es->rooms[0].name, "wtf nigger", MB_OK);
    es->roomCount = rooms.size();
    es->gmObjects =  objects.size() > 0 ? objects[0] : NULL;
    es->gmObjectCount = objects.size();
    es->scripts = objects.size() > 0 ? scripts[0] : NULL;
    es->scriptCount = scripts.size();
    es->shaders = shaders.size() > 0 ? shaders[0] : NULL;
    es->shaderCount = shaders.size();
    es->fonts = fonts.size() > 0 ? fonts[0] : NULL;
    es->fontCount = fonts.size();
    es->sprites = sprites.size() > 0 ? sprites[0] : NULL;
    es->spriteCount = sprites.size();
    es->timelines = timelines.size() > 0 ? timelines[0] : NULL;
    es->timelineCount = timelines.size();
    es->backgrounds = backgrounds.size() > 0 ? backgrounds[0] : NULL;
    es->backgroundCount = backgrounds.size();
    cout << compileEGMf(es, output, emode_run) << endl;
}

void buildgmk() {
    //Gmk::GmkFile* gmk = new Gmk::GmkFile();
    //gmk->Load(argv[1]);
    
    // copy gmk to enigma resources and call compile
    
    //delete gmk;
}

int main(int argc, char* argv[])
{
    cout << "ENIGMA Make" << endl;
    
    chdir("../../");
    
    void *result = LoadPluginLib();

    if (result == NULL)
    {
      cout << "ERROR! Failed to communicate with the plugin. Have you compiled and built ENIGMA yet?" << endl;
      cout << "Press any key to continue";
      cin.ignore(1);
    }
    
    libSetMakeDirectory("C:/ProgramData/ENIGMA/");
    
    InitializePluginLib();

    definitionsModified("", ((const char*) "%e-yaml\n"
    "---\n"
    "treat-literals-as: 0\n"
    "sample-lots-of-radios: 0\n"
    "inherit-equivalence-from: 0\n"
    "make-directory: \"%PROGRAMDATA%/ENIGMA/\"\n"
    "sample-checkbox: on\n"
    "sample-edit: DEADBEEF\n"
    "sample-combobox: 0\n"
    "inherit-strings-from: 0\n"
    "inherit-negatives-as: 0\n"
    "inherit-escapes-from: 0\n"
    "inherit-objects: true \n"
    "inherit-increment-from: 0\n"
    " \n"
    "target-audio: OpenAL\n"
    "target-windowing: Win32\n"
    "target-compiler: gcc\n"
    "target-graphics: OpenGL1\n"
    "target-widget: None\n"
    "target-collision: BBox\n"
    "target-networking: None\n"
    "extensions: Universal_System/Extensions/Paths\n"
    ));

    // obtain keywords
    const char* currentResource = (const char*) first_available_resource();
    while (!resources_atEnd())
    {
        currentResource = next_available_resource();
    }

    bool closing = false;
    string input = "";
    while (!closing) {
      cout << "What would you like to do?" << endl;
      getline(cin, input);
      if (strcmp(input.c_str(), "quit") == 0) {
         closing = true;
      } else if (strcmp(input.c_str(), "test") == 0) {
        buildtestproject("C:/Users/Owner/Desktop/wtf.exe");
      } else if (strcmp(input.c_str(), "gmx") == 0) {
        buildgmx("C:/Users/Owner/Desktop/test.gmx/test.project.gmx", "C:/Users/Owner/Desktop/wtf.exe");
      }
    }

    return 0;
}
