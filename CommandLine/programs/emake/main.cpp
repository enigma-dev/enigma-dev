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
#include <sstream>
#include <string>
#include <vector>
#include <map>
using std::vector;
using std::map;
#include "library.h"

#include "Universal_System/zlib.h"
#include "Universal_System/lodepng.h"

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

unsigned char* readpngimg(string filename, unsigned int* width, unsigned int* height, unsigned int* datasize, bool flipped) {
	unsigned error;
	unsigned char* image;
	unsigned pngwidth, pngheight;

	error = lodepng_decode32_file(&image, &pngwidth, &pngheight, filename.c_str());
	if (error)
	{
	  printf("error %u: %s\n", error, lodepng_error_text(error));
	  return NULL;
	}

	unsigned ih,iw;
	const unsigned int bitmap_size = pngwidth*pngheight*4;
	unsigned char* bitmap = new unsigned char[bitmap_size](); // Initialize to zero.
  
	for (ih = 0; ih < pngheight; ih++) {
	  unsigned tmp = 0;
	  if (!flipped) {
      tmp = ih*pngwidth*4;
	  } else {
      tmp = (pngheight - 1 - ih)*pngwidth*4;
	  }
	  for (iw = 0; iw < pngwidth; iw++) {
      bitmap[tmp+0] = image[4*pngwidth*ih+iw*4+2];
      bitmap[tmp+1] = image[4*pngwidth*ih+iw*4+1];
      bitmap[tmp+2] = image[4*pngwidth*ih+iw*4+0];
      bitmap[tmp+3] = image[4*pngwidth*ih+iw*4+3];
      tmp+=4;
	  }
	}

	free(image);
	*width  = pngwidth;
	*height = pngheight;
  *datasize = bitmap_size;
	return bitmap;
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

vector<Script> scripts;
vector<Sprite> sprites;
vector<Shader> shaders;
vector<Sound> sounds;
vector<Font> fonts;
vector<Background> backgrounds;
vector<Path> paths;
vector<Timeline> timelines;
vector<GmObject> objects;
vector<Room> rooms;

Sprite* readGMXSprite(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".sprite.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Sprite* spr = new Sprite();
  spr->name = strcpy(new char[name.size() + 1],name.c_str());
  spr->id = sprites.size();
  
  spr->bbMode = atoi(pRoot->first_node("bboxmode")->value());
  spr->bbLeft = atoi(pRoot->first_node("bbox_left")->value());
  spr->bbRight = atoi(pRoot->first_node("bbox_right")->value());
  spr->bbTop = atoi(pRoot->first_node("bbox_top")->value());
  spr->bbBottom = atoi(pRoot->first_node("bbox_bottom")->value());
  spr->separateMask = atoi(pRoot->first_node("sepmasks")->value()) < 0;

  int width = atoi(pRoot->first_node("width")->value()),
      height =  atoi(pRoot->first_node("height")->value());

  vector<SubImage> subimages;
  for (xml_node<> *imgnode = pRoot->first_node("frames")->first_node("frame"); imgnode; imgnode = imgnode->next_sibling())
  {
    unsigned int imgw, imgh, imgs;
    unsigned char* imgdata = readpngimg(filepath.substr(0, filepath.find_last_of('/') + 1) + string_replace_all(imgnode->value(), "\\", "/"), &imgw, &imgh, &imgs, false);
    if (imgdata == NULL) {
      return NULL;
    }
  
    SubImage subimg;
    subimg.image.width = width;
    subimg.image.height = height;
    subimg.image.data = reinterpret_cast<char*>(zlib_compress(imgdata, imgs));
    subimg.image.dataSize = width*height*4;
    subimages.push_back(subimg);
  }

  spr->subImages = new SubImage[subimages.size()];
  copy(subimages.begin(), subimages.end(), spr->subImages);
  spr->subImageCount = subimages.size();
  
  doc.clear();

  return spr;
}

Background* readGMXBackground(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".background.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Background* bkg = new Background();
  bkg->name = strcpy(new char[name.size() + 1],name.c_str());
  bkg->id = backgrounds.size();
  
  bkg->hOffset = atoi(pRoot->first_node("tilexoff")->value());
  bkg->vOffset = atoi(pRoot->first_node("tileyoff")->value());
  bkg->hSep = atoi(pRoot->first_node("tilehsep")->value());
  bkg->vSep = atoi(pRoot->first_node("tilevsep")->value());
  bkg->tileWidth = atoi(pRoot->first_node("tilewidth")->value());
  bkg->tileHeight = atoi(pRoot->first_node("tileheight")->value());
  bkg->useAsTileset = atoi(pRoot->first_node("istileset")->value()) < 0;
  bkg->transparent = 0;
  bkg->smoothEdges = 0;
  bkg->preload = 0;
  
  unsigned int imgw, imgh, imgs;
  unsigned char* imgdata = readpngimg(filepath.substr(0, filepath.find_last_of('/') + 1) + string_replace_all(pRoot->first_node("data")->value(), "\\", "/"), &imgw, &imgh, &imgs, false);
  if (imgdata == NULL) {
    return NULL;
  }
  
  bkg->backgroundImage.width = atoi(pRoot->first_node("width")->value());
  bkg->backgroundImage.height = atoi(pRoot->first_node("height")->value());
  bkg->backgroundImage.data = reinterpret_cast<char*>(zlib_compress(imgdata, imgs));
  bkg->backgroundImage.dataSize = imgs;
  
  doc.clear();
  
  return bkg;
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
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".path.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Path* pth = new Path();
  pth->name = strcpy(new char[name.size() + 1],name.c_str());
  pth->id = paths.size();
  
  doc.clear();
  
  return pth;
}

Timeline* readGMXTimeline(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".timeline.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Timeline* tml = new Timeline();
  tml->name = strcpy(new char[name.size() + 1],name.c_str());
  tml->id = timelines.size();
  
  doc.clear();
  
  return tml;
}

GmObject* readGMXObject(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".object.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  GmObject* obj = new GmObject();
  obj->name = strcpy(new char[name.size() + 1],name.c_str());
  obj->id = objects.size();
  
  //TODO: Find these, use postponed reference for parent.
  obj->spriteId = -1;
  obj->maskId = -1;
  obj->parentId = -100;
  obj->solid = atoi(pRoot->first_node("solid")->value()) < 0;
  obj->visible = atoi(pRoot->first_node("visible")->value()) < 0;
  obj->persistent = atoi(pRoot->first_node("persistent")->value()) < 0;
  obj->depth = atoi(pRoot->first_node("depth")->value());
  
  map< int, vector<Event> > events;
  for (xml_node<> *imgnode = pRoot->first_node("events")->first_node("event"); imgnode; imgnode = imgnode->next_sibling())
  {
    unsigned int mid = atoi(imgnode->first_attribute("eventtype")->value()),
                 sid = atoi(imgnode->first_attribute("enumb")->value());
    Event event;
    event.id = sid;
    event.code = "";
    events[mid].push_back(event);
  }
  
  vector<MainEvent> mainevents;
  for (map< int, vector<Event> >::iterator it = events.begin(); it != events.end(); it++) {
    MainEvent mainevent;
    mainevent.id = it->first;
    mainevent.events = new Event[it->second.size()];
    copy(it->second.begin(), it->second.end(), mainevent.events);
    mainevent.eventCount = it->second.size();
    mainevents.push_back(mainevent);
  }

  obj->mainEvents = new MainEvent[mainevents.size()];
  copy(mainevents.begin(), mainevents.end(), obj->mainEvents);
  obj->mainEventCount = mainevents.size();
  
  doc.clear();
  
  return obj;
}

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

void iterateGMXTree(xml_node<> *root, const std::string& folder) {
  for (xml_node<> *node = root->first_node(); node; node = node->next_sibling())
  {
    if (strcmp(node->name(), "sprite") == 0) {
      Sprite* sprite = readGMXSprite( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      sprites.push_back(*sprite);
      delete sprite;
    } else if (strcmp(node->name(), "sound") == 0) {
      Sound* sound = readGMXSound( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      sounds.push_back(*sound);
      delete sound;
    } else if (strcmp(node->name(), "background") == 0) {
      Background* background = readGMXBackground( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      backgrounds.push_back(*background);
      delete background;
    } else if (strcmp(node->name(), "path") == 0) {
      Path* path = readGMXPath( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      paths.push_back(*path);
      delete path;
    } else if (strcmp(node->name(), "script") == 0) {
      Script* script = readGMXScript( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      scripts.push_back(*script);
      delete script;
    } else if (strcmp(node->name(), "shader") == 0) {
      Shader* shader = readGMXShader( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      shaders.push_back(*shader);
      delete shader;
    } else if (strcmp(node->name(), "font") == 0) {
      Font* font = readGMXFont( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      fonts.push_back(*font);
      delete font;
    //NOTE: Read timelines followed by objects followed by rooms, because each can refer to each other in that order and it cuts down on unnecessary postponed references.
    } else if (strcmp(node->name(), "timeline") == 0) {
      Timeline* timeline = readGMXTimeline( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      timelines.push_back(*timeline);
      delete timeline;
    } else if (strcmp(node->name(), "object") == 0) {
      GmObject* object = readGMXObject( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      objects.push_back(*object);
      delete object;
    } else if (strcmp(node->name(), "room") == 0) {
      Room* room = readGMXRoom( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      rooms.push_back(*room);
      delete room;
    } else {
      iterateGMXTree(node, folder);
    }
  }
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
    
    iterateGMXTree(pRoot, folder);
    
    es->rooms = &rooms[0];
    es->roomCount = rooms.size();
    es->gmObjects =  &objects[0];
    es->gmObjectCount = objects.size();
    es->sprites = &sprites[0];
    es->spriteCount = sprites.size();
    es->scripts = &scripts[0];
    es->scriptCount = scripts.size();
    es->sounds = &sounds[0];
    es->soundCount = sounds.size();
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
