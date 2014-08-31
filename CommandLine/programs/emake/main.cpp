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

unsigned int BGR2RGBA(unsigned int x) {
  return (((x & 0xFF0000) >> 16) | (x & 0xFF00) | ((x & 0xFF) << 16)) << 8 | 0xFF;
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

static void string_split(std::vector<std::string>& lst, const std::string& input, const std::string& separators, bool remove_empty = true)
{
    std::ostringstream word;
    for (size_t n = 0; n < input.size(); ++n)
    {
        if (std::string::npos == separators.find(input[n]))
            word << input[n];
        else
        {
            if (!word.str().empty() || !remove_empty)
                lst.push_back(word.str());
            word.str("");
        }
    }
    if (!word.str().empty() || !remove_empty)
        lst.push_back(word.str());
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
vector<GameSettings> gamesettings;

unsigned lastInstanceId = 100000,
         lastTileId = 10000000;

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
  
  spr->originX = atoi(pRoot->first_node("xorig")->value());
  spr->originY = atoi(pRoot->first_node("yorigin")->value());
  spr->bbMode = atoi(pRoot->first_node("bboxmode")->value());
  spr->bbLeft = atoi(pRoot->first_node("bbox_left")->value());
  spr->bbRight = atoi(pRoot->first_node("bbox_right")->value());
  spr->bbTop = atoi(pRoot->first_node("bbox_top")->value());
  spr->bbBottom = atoi(pRoot->first_node("bbox_bottom")->value());
  spr->separateMask = atoi(pRoot->first_node("sepmasks")->value());

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
  bkg->useAsTileset = atoi(pRoot->first_node("istileset")->value());
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
  
  snd->preload = atoi(pRoot->first_node("preload")->value());
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
  
  pCurrentNode = pRoot->first_node("name");
  fnt->fontName = strcpy(new char[pCurrentNode->value_size() + 1],pCurrentNode->value());
  fnt->size = atoi(pRoot->first_node("size")->value());
  fnt->bold = atoi(pRoot->first_node("bold")->value());
  fnt->italic = atoi(pRoot->first_node("italic")->value());
  
  //TODO: Read glyphs and ranges.
  
  doc.clear();
  
  return fnt;
}

Script* readGMXScript(const char* path) {
  string filepath = path;
  size_t start = filepath.find_last_of('/') + 1;
  size_t end = filepath.find_last_of('.');
  string name = filepath.substr(start, end - start);
  string content = readtxtfile((string(path)).c_str());
  
  Script* scr = new Script();
  scr->name = strcpy(new char[name.size() + 1],name.c_str());
  scr->id = scripts.size();
  
  scr->code = strcpy(new char[content.size() + 1],content.c_str());

  return scr;
}

Shader* readGMXShader(const char* path) {
  string filepath = path;
  size_t start = filepath.find_last_of('/') + 1;
  size_t end = filepath.find_last_of('.');
  string name = filepath.substr(start, end - start);
  string content = readtxtfile((string(path)).c_str()); // extension is sent in the path

  Shader* shr = new Shader();
  shr->name = strcpy(new char[name.size() + 1],name.c_str());
  shr->id = shaders.size();
  
  vector<string> split;
  string_split(split, content, "//######################_==_YOYO_SHADER_MARKER_==_######################@~", true);
  
  shr->vertex = strcpy(new char[split[0].size() + 1],split[0].c_str());
  shr->fragment = strcpy(new char[split[1].size() + 1],split[1].c_str());
  shr->precompile = true;
  
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
  
  pth->precision = atoi(pRoot->first_node("precision")->value());
  pth->closed = atoi(pRoot->first_node("closed")->value());
  
  vector<PathPoint> points;
  for (xml_node<> *pntnode = pRoot->first_node("points")->first_node("point"); pntnode; pntnode = pntnode->next_sibling())
  {
    vector<string> split;
    string_split(split, pntnode->value(), ",", true);

    PathPoint point;
    point.x = atoi(split[0].c_str());
    point.y = atoi(split[1].c_str());
    point.speed = atoi(split[2].c_str());
    points.push_back(point);
  }

  pth->points = new PathPoint[points.size()];
  copy(points.begin(), points.end(), pth->points);
  pth->pointCount = points.size();
  
  doc.clear();
  
  return pth;
}

enum ArgumentKind {
  ARG_EXPRESSION = 0,
  ARG_STRING = 1,
  ARG_BOTH = 2,
  ARG_BOOLEAN = 3,
  ARG_MENU = 4,
  ARG_COLOR = 13
};

enum ActionKind {
  ACT_NORMAL = 0,
  ACT_BEGIN = 1,
  ACT_END = 2,
  ACT_ELSE = 3,
  ACT_EXIT = 4,
  ACT_REPEAT = 5,
  ACT_VARIABLE = 6,
  ACT_CODE = 7,
  ACT_PLACEHOLDER = 8,
  ACT_SEPARATOR = 9,
  ACT_LABEL = 10
};

enum ExecutionType {
  EXEC_NONE = 0,
  EXEC_FUNCTION = 1,
  EXEC_CODE = 2
};

enum AppliesTo {
  OBJECT_SELF = -1,
  OBJECT_OTHER = -2,
};

struct Argument {
  unsigned int kind;
  string val;
  
  Argument(): kind(0), val("") {
  
  }
  
  ~Argument() {
    
  }
};

string argumentToString(const Argument& arg) {
  if (arg.val.length() == 0) {
    if (arg.kind == ARG_STRING) {
      return "\"\"";
    }
    return "0";
  }
  
  switch (arg.kind)
  {
    case ARG_BOTH:
      //treat as literal if starts with quote (")
      if (arg.val[0] == '\"' || arg.val[0] == '\'') return arg.val;
      //else fall through
    case ARG_STRING:
      return "\"" + string_replace_all(string_replace_all(arg.val,"\\","\\\\"),"\"","\"+'\"'+\"") + "\"";
    case ARG_BOOLEAN:
      return (strcmp(arg.val.c_str(), "0") != 0) ? "true":"false";
    case ARG_MENU:
      return arg.val;
    case ARG_COLOR: {
      char buf[32]; 
      sprintf(buf,"%06X", atoi(arg.val.c_str()));
      return buf;
    }
    default:
      return arg.val;
  }
}

static bool actionDemise = false; //when unsupported actions are encountered, only report 1 error
static int numberOfBraces = 0; //gm ignores brace actions which are in the wrong place or missing
static int numberOfIfs = 0; //gm allows multipe else actions after 1 if, so its important to track the number

string readGMXActionSequence(xml_node<> *root) {
  string nl = "\n";
  string code = "";

  numberOfBraces = 0;
  numberOfIfs = 0;
  
  for (xml_node<> *actnode = root->first_node("action"); actnode; actnode = actnode->next_sibling())
  {
    unsigned int actlibid = atoi(actnode->first_node("libid")->value());
    unsigned int actid = atoi(actnode->first_node("id")->value());
    unsigned int actkind = atoi(actnode->first_node("kind")->value());
    unsigned int actexectype = atoi(actnode->first_node("exetype")->value());
    bool actuserelative = atoi(actnode->first_node("userelative")->value());
    bool actuseapplyto = atoi(actnode->first_node("useapplyto")->value());
    bool actisquestion = atoi(actnode->first_node("isquestion")->value());
    bool actrelative = atoi(actnode->first_node("relative")->value());
    bool actisnot = atoi(actnode->first_node("isnot")->value());
    
    string actexecinfo = "";
    if (actexectype == EXEC_FUNCTION) {
      actexecinfo = actnode->first_node("functionname")->value();
    } if (actexectype == EXEC_CODE) {
      actexecinfo = actnode->first_node("codestring")->value();
    }
    
    vector<Argument> args;
    for (xml_node<> *argnode = actnode->first_node("arguments")->first_node("argument"); argnode; argnode = argnode->next_sibling())
    {
      Argument arg;
      for (xml_node<> *argpnode = argnode->first_node(); argpnode; argpnode = argpnode->next_sibling())
      {
        if (strcmp(argpnode->name(), "kind") == 0) {
          arg.kind = atoi(argpnode->value());
        } else if (strcmp(argpnode->name(), "string") == 0) {
          arg.val = string(argpnode->value());
        } else {
          arg.val = string(argpnode->value()); //TODO: Get the id, Josh said to avoid resources of different types with the same name switch(arg.kind)
        }
      }
      args.push_back(arg);
    }
    
      switch (actkind)
      {
        case ACT_BEGIN:
          code.append("{");
          numberOfBraces++;
          break;
        case ACT_CODE:
          //surround with brackets (e.g. for if conditions before it) and terminate dangling comments
          code.append("{").append(args[0].val).append("/**/\n}").append(nl);
          break;
        case ACT_ELSE:
          {
          if (numberOfIfs > 0)
            {
            code.append("else ");
            numberOfIfs--;
            }
          }
          break;
        case ACT_END:
          if (numberOfBraces > 0)
            {
            code.append("}");
            numberOfBraces--;
            }
          break;
        case ACT_EXIT:
          code.append("exit ");
          break;
        case ACT_REPEAT:
          code.append("repeat (").append(args[0].val).append(") ");
          break;
        case ACT_VARIABLE:
          if (actrelative)
            code.append(args[0].val).append(" += ").append(args[1].val).append(nl);
          else
            code.append(args[0].val).append(" = ").append(args[1].val).append(nl);
          break;
        case ACT_NORMAL:
          {
          if (actexectype == EXEC_NONE) break;
          const char* appliesto = actnode->first_node("whoName")->value();
          int apto = -3;
          if (strcmp(appliesto, "self") == 0) {
            apto = OBJECT_SELF; 
          } else if (strcmp(appliesto, "other") == 0) {
            apto = OBJECT_OTHER;
          } else {
            for (size_t i = 0; i < objects.size(); i++) {
              if (strcmp(objects[i].name,appliesto) == 0) {
                apto = i; break;
              }
            }
          }
          if (apto != OBJECT_SELF)
          {
            if (actisquestion)
            {
              /* Question action using with statement */
              if (apto == OBJECT_OTHER) {
                code.append("with (other) ");
              } else if (apto != -3) {
                stringstream ss;
                ss << "with (" << apto << ") "; 
                code.append(ss.str());
              } else {
                code.append("/*null with!*/");
              }
            }
            else
            {
              if (apto == OBJECT_OTHER) {
                code.append("with (other) {");
              } else if (apto != -3) {
                stringstream ss;
                ss << "with (" << apto << ") {";
                code.append(ss.str());
              } else {
                code.append("/*null with!*/{");
              }
            }
          }
          if (actisquestion)
          {
            code.append("if ");
            numberOfIfs++;
          }
          if (actisnot) code.append("!");
          if (actuserelative)
          {
            if (actisquestion)
              code.append("(argument_relative := ").append(actrelative ? "true" : "false").append(", ");
            else
              code.append("{argument_relative := ").append(actrelative ? "true" : "false").append("; ");
          }
          if (actisquestion && actexectype == EXEC_CODE) {
            stringstream ss;
            ss << "lib" << actlibid << "_action" << actid;
            code.append(ss.str());
          } else {
            code.append(actexecinfo); 
          }
          if (actexectype == EXEC_FUNCTION)
            {
            code.append("(");
            for (int i = 0; i < args.size(); i++)
              {
              if (i != 0) code.append(",");
              code.append(argumentToString(args[i]));
              }
            code.append(")");
            }
          if (actuserelative) code.append(actisquestion ? ")" : "\n}");
          code.append(nl);

          if (apto != OBJECT_SELF && (!actisquestion))
            code.append("\n}");
          }
          break;   
      }
    }
  
  if (numberOfBraces > 0)
  {
    //someone forgot the closing block action
    for (int i = 0; i < numberOfBraces; i++)
      code.append("\n}");
  }

  return code;
}

Timeline* readGMXTimeline(const char* path, size_t id) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".timeline.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  Timeline* tml = &timelines[id];
  //tml->name = strcpy(new char[name.size() + 1],name.c_str());
  //tml->id = timelines.size();
  
  vector<Moment> moments;
  for (xml_node<> *momnode = pRoot->first_node("entry"); momnode; momnode = momnode->next_sibling())
  {
    unsigned int stepnum = atoi(momnode->first_node("step")->value());
    
    Moment moment;
    moment.stepNo = stepnum;
    string code = readGMXActionSequence(momnode->first_node("event"));
    moment.code = strcpy(new char[code.size() + 1], code.c_str());
    moments.push_back(moment);
  }
  
  tml->moments = new Moment[moments.size()];
  copy(moments.begin(), moments.end(), tml->moments);
  tml->momentCount = moments.size();
  
  doc.clear();
  
  return tml;
}

GmObject* readGMXObject(const char* path, size_t id) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".object.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pCurrentNode;

  GmObject* obj = &objects[id];
  //obj->name = strcpy(new char[name.size() + 1],name.c_str());
  //obj->id = objects.size();
  obj->spriteId = -1;
  char* spritename = pRoot->first_node("spriteName")->value();
  if (strcmp(spritename, "<undefined>") != 0) {
    for (size_t i = 0; i < sprites.size(); i++) {
      if (strcmp(sprites[i].name,spritename) == 0) {
        obj->spriteId = i; break;
      }
    }
  }
  obj->maskId = -1;
  char* maskname = pRoot->first_node("maskName")->value();
  if (strcmp(maskname, "<undefined>") != 0) {
    for (size_t i = 0; i < sprites.size(); i++) {
      if (strcmp(sprites[i].name,maskname) == 0) {
        obj->maskId = i; break;
      }
    }
  }
  
  obj->parentId = -100;
  char* parentname = pRoot->first_node("parentName")->value();
  if (strcmp(parentname, "<undefined>") != 0) {
    for (size_t i = 0; i < objects.size(); i++) {
      if (strcmp(objects[i].name,parentname) == 0) {
        obj->parentId = i; break;
      }
    }
  }
  obj->solid = atoi(pRoot->first_node("solid")->value());
  obj->visible = atoi(pRoot->first_node("visible")->value());
  obj->persistent = atoi(pRoot->first_node("persistent")->value());
  obj->depth = atoi(pRoot->first_node("depth")->value());
  
  map< int, vector<Event> > events;
  for (xml_node<> *evtnode = pRoot->first_node("events")->first_node("event"); evtnode; evtnode = evtnode->next_sibling())
  {
    unsigned int mid = atoi(evtnode->first_attribute("eventtype")->value()),
                 sid = -1;
    xml_attribute<>* enameattr = evtnode->first_attribute("ename");
    if (enameattr != NULL) {
      //continue;
      char* ename = enameattr->value();
      if (strcmp(ename, "<undefined>") != 0) {
        for (size_t i = 0; i < objects.size(); i++) {
          if (strcmp(objects[i].name,ename) == 0) {
            sid = i; break;
          }
        }
      }
    } else {
      sid = atoi(evtnode->first_attribute("enumb")->value());
    }

    Event event;
    event.id = sid;
    string code = readGMXActionSequence(evtnode);
    event.code = strcpy(new char[code.size() + 1], code.c_str());
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
  
  rm->drawBackgroundColor = atoi(pRoot->first_node("showcolour")->value());
  rm->enableViews = atoi(pRoot->first_node("enableViews")->value());
  rm->width = atoi(pRoot->first_node("width")->value());
  rm->height = atoi(pRoot->first_node("height")->value());
  pCurrentNode = pRoot->first_node("code");
  rm->creationCode = strcpy(new char[pCurrentNode->value_size() + 1],pCurrentNode->value());
  rm->speed = atoi(pRoot->first_node("speed")->value());
  pCurrentNode = pRoot->first_node("caption");
  rm->caption = strcpy(new char[pCurrentNode->value_size() + 1],pCurrentNode->value());
  rm->instanceCount = 0;
  rm->backgroundColor = BGR2RGBA(atol(pRoot->first_node("colour")->value()));//RGBA2DWORD(3, 149, 255, 255);
  
  vector<BackgroundDef> backgrounddefs;
  for (xml_node<> *bnode = pRoot->first_node("backgrounds")->first_node("background"); bnode; bnode = bnode->next_sibling())
  {
    BackgroundDef backgrounddef;
    
    backgrounddef.backgroundId = -1;
    char* backgroundname = bnode->first_attribute("name")->value();
    if (strcmp(backgroundname, "<undefined>") != 0) {
      for (size_t i = 0; i < backgrounds.size(); i++) {
        if (strcmp(backgrounds[i].name,backgroundname) == 0) {
          backgrounddef.backgroundId = i; break;
        }
      }
    }

    backgrounddef.visible = atoi(bnode->first_attribute("visible")->value());
    backgrounddef.foreground = atoi(bnode->first_attribute("foreground")->value());
    backgrounddef.x = atoi(bnode->first_attribute("x")->value());
    backgrounddef.y = atoi(bnode->first_attribute("y")->value());
    backgrounddef.tileHoriz = atoi(bnode->first_attribute("htiled")->value());
    backgrounddef.tileVert = atoi(bnode->first_attribute("vtiled")->value());
    backgrounddef.hSpeed = atoi(bnode->first_attribute("hspeed")->value());
    backgrounddef.vSpeed = atoi(bnode->first_attribute("vspeed")->value());
    backgrounddef.stretch = atoi(bnode->first_attribute("stretch")->value());

    backgrounddefs.push_back(backgrounddef);
  }
  rm->backgroundDefs = new BackgroundDef[backgrounddefs.size()];
  copy(backgrounddefs.begin(), backgrounddefs.end(), rm->backgroundDefs);
  rm->backgroundDefCount = backgrounddefs.size();
  
  vector<View> views;
  for (xml_node<> *vnode = pRoot->first_node("views")->first_node("view"); vnode; vnode = vnode->next_sibling())
  {
    View view;
    
    view.objectId = -1;
    char* objectname = vnode->first_attribute("objName")->value();
    if (strcmp(objectname, "<undefined>") != 0) {
      for (size_t i = 0; i < objects.size(); i++) {
        if (strcmp(objects[i].name,objectname) == 0) {
          view.objectId = i; break;
        }
      }
    }

    view.viewX = atoi(vnode->first_attribute("xview")->value());
    view.viewY = atoi(vnode->first_attribute("yview")->value());
    view.viewW = atoi(vnode->first_attribute("wview")->value());
    view.viewH = atoi(vnode->first_attribute("hview")->value());
    view.portX = atoi(vnode->first_attribute("xport")->value());
    view.portY = atoi(vnode->first_attribute("yport")->value());
    view.portW = atoi(vnode->first_attribute("wport")->value());
    view.portH = atoi(vnode->first_attribute("hport")->value());
    view.borderH = atoi(vnode->first_attribute("hborder")->value());
    view.borderV = atoi(vnode->first_attribute("vborder")->value());
    view.speedH = atoi(vnode->first_attribute("hspeed")->value());
    view.speedV = atoi(vnode->first_attribute("vspeed")->value());
    view.visible = atoi(vnode->first_attribute("visible")->value());
    views.push_back(view);
  }
  rm->views = new View[views.size()];
  copy(views.begin(), views.end(), rm->views);
  rm->viewCount = views.size();
  
  vector<Instance> instances;
  for (xml_node<> *instnode = pRoot->first_node("instances")->first_node("instance"); instnode; instnode = instnode->next_sibling())
  {
    Instance instance;
    
    instance.objectId = -1;
    char* objectname = instnode->first_attribute("objName")->value();
    if (strcmp(objectname, "<undefined>") != 0) {
      for (size_t i = 0; i < objects.size(); i++) {
        if (strcmp(objects[i].name,objectname) == 0) {
          instance.objectId = i; break;
        }
      }
    }
    
    instance.id = lastInstanceId++;
    instance.x = atoi(instnode->first_attribute("x")->value());
    instance.y = atoi(instnode->first_attribute("y")->value());
    instance.locked = atoi(instnode->first_attribute("locked")->value());
    string createCode = instnode->first_attribute("code")->value();
    instance.creationCode = strcpy(new char[createCode.size() + 1], createCode.c_str());
    instances.push_back(instance);
  }
  rm->instances = new Instance[instances.size()];
  copy(instances.begin(), instances.end(), rm->instances);
  rm->instanceCount = instances.size();
  
  vector<Tile> tiles;
  for (xml_node<> *tnode = pRoot->first_node("tiles")->first_node("tile"); tnode; tnode = tnode->next_sibling())
  {
    Tile tile;
    
    tile.backgroundId = -1;
    char* backgroundname = tnode->first_attribute("name")->value();
    if (strcmp(backgroundname, "<undefined>") != 0) {
      for (size_t i = 0; i < backgrounds.size(); i++) {
        if (strcmp(backgrounds[i].name,backgroundname) == 0) {
          tile.backgroundId = i; break;
        }
      }
    }

    tile.bgX = atoi(tnode->first_attribute("xo")->value());
    tile.bgY = atoi(tnode->first_attribute("yo")->value());
    tile.roomX = atoi(tnode->first_attribute("x")->value());
    tile.roomY = atoi(tnode->first_attribute("y")->value());
    tile.width = atoi(tnode->first_attribute("w")->value());
    tile.height = atoi(tnode->first_attribute("h")->value());
    tile.depth = atoi(tnode->first_attribute("depth")->value());
    tile.locked = atoi(tnode->first_attribute("locked")->value());
    tile.id = lastTileId++;// atoi(tnode->first_attribute("id")->value()); <---- TODO: GMX does store the id, but for the purpose of obtaining the last tile id a local was used
    
    tiles.push_back(tile);
  }
  rm->tiles = new Tile[tiles.size()];
  copy(tiles.begin(), tiles.end(), rm->tiles);
  rm->tileCount = tiles.size();
  
  doc.clear();
  
  return rm;
}

GameSettings* readGMXSettings(const char* path) {
  string filepath = path;
  string name = filepath.substr(filepath.find_last_of('/') + 1, filepath.length());
  string content = readtxtfile((string(path) + ".config.gmx").c_str());
  xml_document<> doc;    // character type defaults to char
  doc.parse<0>(&content[0]);    // 0 means default parse flags
  xml_node<> *pRoot = doc.first_node()->first_node("Options");
  
  GameSettings* gs = new GameSettings();
  string icopath = filepath.append("/windows/runner_icon.ico");
  gs->gameIcon = strcpy(new char[icopath.size() + 1], icopath.c_str());
  gs->letEscEndGame = strcmp(pRoot->first_node("option_closeesc")->value(), "true") == 0;
  gs->treatCloseAsEscape = strcmp(pRoot->first_node("option_closeesc")->value(), "true") == 0;
  gs->alwaysOnTop = strcmp(pRoot->first_node("option_stayontop")->value(), "true") == 0;
  gs->abortOnError = strcmp(pRoot->first_node("option_aborterrors")->value(), "true") == 0;
  gs->allowWindowResize = strcmp(pRoot->first_node("option_sizeable")->value(), "true") == 0;
  gs->freezeOnLoseFocus = strcmp(pRoot->first_node("option_freeze")->value(), "true") == 0;
  gs->setResolution = strcmp(pRoot->first_node("option_changeresolution")->value(), "true") == 0;
  gs->dontDrawBorder = strcmp(pRoot->first_node("option_noborder")->value(), "true") == 0;
  gs->dontShowButtons = strcmp(pRoot->first_node("option_nobuttons")->value(), "true") == 0;
  gs->displayCursor = strcmp(pRoot->first_node("option_showcursor")->value(), "true") == 0;
  gs->interpolate = strcmp(pRoot->first_node("option_interpolate")->value(), "true") == 0;
  gs->disableScreensavers = strcmp(pRoot->first_node("option_noscreensaver")->value(), "true") == 0;
  
  gs->gameId = atoi(pRoot->first_node("option_gameid")->value());
  
  gs->versionMajor = atoi(pRoot->first_node("option_version_major")->value());
	gs->versionMinor = atoi(pRoot->first_node("option_version_minor")->value());
	gs->versionRelease = atoi(pRoot->first_node("option_version_release")->value());
	gs->versionBuild = atoi(pRoot->first_node("option_version_build")->value());
  
  string author = pRoot->first_node("option_author")->value();
  gs->author = strcpy(new char[author.size() + 1], author.c_str());
  string company = pRoot->first_node("option_version_company")->value();
  gs->company = strcpy(new char[company.size() + 1], company.c_str());
  string description = pRoot->first_node("option_version_description")->value();
  gs->description = strcpy(new char[description.size() + 1], description.c_str());
  //string version = pRoot->first_node("option_version")->value();
  gs->version = "";
  string product = pRoot->first_node("option_version_product")->value();
  gs->product = strcpy(new char[product.size() + 1], product.c_str());
  string copyright = pRoot->first_node("option_version_copyright")->value();
  gs->copyright = strcpy(new char[copyright.size() + 1], copyright.c_str());
  return gs;
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
      string type = node->first_attribute("type")->value();
      shader->type = strcpy(new char[type.size() + 1], type.c_str());
      shaders.push_back(*shader);
      delete shader;
    } else if (strcmp(node->name(), "font") == 0) {
      Font* font = readGMXFont( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      fonts.push_back(*font);
      delete font;
    //NOTE: Read timelines followed by objects followed by rooms, because each can refer to each other in that order and it cuts down on unnecessary postponed references.
    } else if (strcmp(node->name(), "timeline") == 0) {
      string name = string_replace_all(node->value(), "\\", "/");
      name = name.substr(name.find_last_of("/") + 1, name.length());
      Timeline* timeline = new Timeline();
      timeline->name = strcpy(new char[name.size() + 1], name.c_str());
      timeline->id = timelines.size();
      timelines.push_back(*timeline);
      delete timeline;
    } else if (strcmp(node->name(), "object") == 0) {
      string name = string_replace_all(node->value(), "\\", "/");
      name = name.substr(name.find_last_of("/") + 1, name.length());
      GmObject* object = new GmObject();
      object->name = strcpy(new char[name.size() + 1], name.c_str());
      object->id = objects.size();
      objects.push_back(*object);
      delete object;
    } else if (strcmp(node->name(), "room") == 0) {
      Room* room = readGMXRoom( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      rooms.push_back(*room);
      delete room;
    } else if (strcmp(node->name(), "Config") == 0) {
      GameSettings* gs = readGMXSettings( (folder + string_replace_all(node->value(), "\\", "/")).c_str() );
      gamesettings.push_back(*gs);
      delete gs;
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
    es->filename = input; // NOTE: This is what LGM does; emulates GM8.1
    es->gameInfo.gameInfoStr = "";
    es->gameInfo.formCaption = "";
    
    iterateGMXTree(pRoot, folder);
    
    // Handle postponed resources.
    for (size_t i = 0; i < objects.size(); i++) {
      readGMXObject((folder + "objects/" + objects[i].name).c_str(), i);
    }
    for (size_t i = 0; i < timelines.size(); i++) {
      readGMXTimeline((folder + "timelines/" + timelines[i].name).c_str(), i);
    }
    
    es->gameSettings = gamesettings[0];
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
    es->lastTileId = lastTileId;
    es->lastInstanceId = lastInstanceId;
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
    "target-widget: Win32\n"
    "target-collision: Precise\n"
    "target-networking: None\n"
    "extensions: Universal_System/Extensions/Alarms,Universal_System/Extensions/DataStructures,Universal_System/Extensions/MotionPlanning,Universal_System/Extensions/Paths\n"
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
        cout << "Enter the location to output the executable:" << endl;
        getline(cin, input);
        buildtestproject(input.c_str());
      } else if (strcmp(input.c_str(), "gmx") == 0) {
        cout << "Enter the file path to the GMX:" << endl;
        getline(cin, input);
        string outputpath;
        cout << "Enter the location to output the executable:" << endl;
        getline(cin, outputpath);
        buildgmx(input.c_str(), outputpath.c_str());
        //buildgmx("C:/Users/Owner/Desktop/test2.gmx/test2.project.gmx", "C:/Users/Owner/Desktop/wtf.exe");
      }
    }

    return 0;
}
