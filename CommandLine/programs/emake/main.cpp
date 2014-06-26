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

std::string readtxtfile(const char* filepath) {
    std::ifstream filestream(filepath);
    std::stringstream buffer;
    buffer << filestream.rdbuf();
    filestream.close();
    return buffer.str();
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

vector<Room> rooms;
vector<GmObject> objects;
vector<Script> scripts;
vector<Sprite> sprites;
vector<Shader> shaders;
vector<Sound> sounds;
vector<Font> fonts;
vector<Timeline> timelines;
vector<Background> backgrounds;
vector<Path> paths;

Room* readGMXRoom(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".room.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Room* rm = new Room();
  rm->name = strcpy(new char[name.size() + 1],name.c_str());
  rm->id = rooms.size();
  
  rm->drawBackgroundColor = atoi(pRoot->first_node("showcolour")->value()) < 0;
  rm->width = atoi(pRoot->first_node("width")->value());
  rm->height = atoi(pRoot->first_node("height")->value());
  pCurrentNode = pRoot->first_node("code");
  rm->creationCode = strcpy(new char[pCurrentNode->value_size() + 1],pCurrentNode->value());
  rm->speed = atoi(pRoot->first_node("speed")->value());
  pCurrentNode = pRoot->first_node("caption");
  rm->caption = strcpy(new char[pCurrentNode->value_size() + 1],pCurrentNode->value());
  rm->instanceCount = 0;
  rm->backgroundColor = atoi(pRoot->first_node("colour")->value());//RGBA2DWORD(3, 149, 255, 255);
  
  doc.clear();
  
  return rm;
}

Sprite* readGMXSprite(const char* path) {

}

GmObject* readGMXObject(const char* path) {

}

Timeline* readGMXTimeline(const char* path) {

}

Sound* readGMXSound(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".sound.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Sound* snd = new Sound();
  snd->name = strcpy(new char[name.size() + 1],name.c_str());
  snd->id = sounds.size();
  
  snd->preload = atoi(pRoot->first_node("preload")->value()) < 0;
  snd->kind = atoi(pRoot->first_node("kind")->value());
  snd->pan = atoi(pRoot->first_node("pan")->value());
  // yes GMX double nests the volume tag
  snd->volume = atoi(pRoot->first_node("volume")->first_node("volume")->value());
  MessageBox(NULL, (filepath.substr(0, filepath.find_last_of('/')) + "/audio/" + pRoot->first_node("data")->value()).c_str(), "wtf", MB_OK);
  // Open sound
  FILE *afile = fopen((filepath.substr(0, filepath.find_last_of('/')) + "/audio/" + pRoot->first_node("data")->value()).c_str(),"rb");
  if (!afile)
    return NULL;

  // Buffer sound
  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  unsigned char *fdata = new unsigned char[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");
  
  snd->data = fdata;
  snd->size = flen;
  
  doc.clear();
  
  return snd;
}

Font* readGMXFont(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".font.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Font* fnt = new Font();
  fnt->name = strcpy(new char[name.size() + 1],name.c_str());
  fnt->id = fonts.size();
  
  pCurrentNode = pRoot->first_node("fontName");
  fnt->fontName = strcpy(new char[pCurrentNode->value_size() + 1],pCurrentNode->value());
  fnt->size = atoi(pRoot->first_node("size")->value());
  fnt->bold = atoi(pRoot->first_node("bold")->value()) < 0;
  fnt->italic = atoi(pRoot->first_node("italic")->value()) < 0;
  
  //TODO: Read glyphs and ranges.
  
  doc.clear();
  
  return fnt;
}

Script* readGMXScript(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".gml").c_str());

  Script* scr = new Script();
  scr->name = strcpy(new char[name.size() + 1],name.c_str());
  scr->id = scripts.size();
  
  scr->code = strcpy(new char[content.size() + 1],content.c_str());

  return scr;
}

Shader* readGMXShader(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".gml").c_str());

  Shader* shr = new Shader();
  shr->name = strcpy(new char[name.size() + 1],name.c_str());
  shr->id = shaders.size();
  
  shr->vertex = strcpy(new char[content.size() + 1],content.c_str());
  shr->fragment = strcpy(new char[content.size() + 1],content.c_str());
  shr->precompile = true;
  //shr->type = ;
  
  return shr;
}

Path* readGMXPath(const char* path) {

}

Background* readGMXBackground(const char* path) {

}

void buildgmx(const char* input, const char* output) {
    xml_document<> doc;    // character type defaults to char
    string content = readtxtfile(input);
    doc.parse<0>(&content[0]);    // 0 means default parse flags
    xml_node<> *pRoot = doc.first_node();
    
    string filepath = input;
    string folder = filepath.substr(0, filepath.find_last_of('/') + 1);

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
          Sound* sound = readGMXSound( (folder + string_replace_all(resnode->value(), "\\", "/")).c_str() );
          sounds.push_back(*sound);
          delete sound;
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
          Script* script = readGMXScript( (folder + string_replace_all(resnode->value(), "\\", "/")).c_str() );
          scripts.push_back(*script);
          delete script;
        }
      } else if (strcmp(node->name(), "shaders") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          Shader* shader = readGMXShader( (folder + string_replace_all(resnode->value(), "\\", "/")).c_str() );
          shaders.push_back(*shader);
          delete shader;
        }
      } else if (strcmp(node->name(), "fonts") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          Font* font = readGMXFont( (folder + string_replace_all(resnode->value(), "\\", "/")).c_str() );
          fonts.push_back(*font);
          delete font;
        }
      } else if (strcmp(node->name(), "rooms") == 0) {
        for (xml_node<> *resnode = node->first_node(); resnode; resnode = resnode->next_sibling())
        {
          Room* room = readGMXRoom( (folder + string_replace_all(resnode->value(), "\\", "/")).c_str() );
          rooms.push_back(*room);
          delete room;
        }
      }
    }
    
    es->rooms = &rooms[0];
    es->roomCount = rooms.size();
    es->gmObjects =  &objects[0];
    es->gmObjectCount = objects.size();
    es->scripts = &scripts[0];
    es->scriptCount = scripts.size();
    es->shaders = &shaders[0];
    es->shaderCount = shaders.size();
    es->fonts = &fonts[0];
    es->fontCount = fonts.size();
    es->sprites = &sprites[0];
    es->spriteCount = sprites.size();
    es->timelines = &timelines[0];
    es->timelineCount = timelines.size();
    es->backgrounds = &backgrounds[0];
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
