/** Copyright (C) 2018 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "assets.h"
#include "lodepng.h"

#include <zlib.h>

#include <map>
#include <vector>

using namespace buffers;
using namespace buffers::resources;
using namespace google::protobuf;

using std::string;
using TypeCase = TreeNode::TypeCase;
using ResourceMap = std::vector<const Message*>;
using TypeMap = std::map<TypeCase, ResourceMap>;

#define irrr(res) if (res) { /*idpr("Error occurred; see scrollback for details.",-1);*/ return res; }

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

static unsigned char* zlib_compress(unsigned char* inbuffer,size_t &actualsize)
{
  uLongf outsize=(int)(actualsize*1.1)+12;
  Bytef* outbytef=new Bytef[outsize];

  compress(outbytef,&outsize,(Bytef*)inbuffer,actualsize);

  actualsize = outsize;

  return (unsigned char*)outbytef;
}

static void write_image(const string& fname, FILE *gameModule) {

  unsigned error;
  unsigned char* image;
  unsigned pngwidth, pngheight;

  error = lodepng_decode32_file(&image, &pngwidth, &pngheight, fname.c_str());
  if (error)
  {
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return;
  }

  unsigned ih,iw;
  const int bitmap_size = pngwidth*pngheight*4;
  unsigned char* bitmap = new unsigned char[bitmap_size](); // Initialize to zero.

  for (ih = 0; ih < pngheight; ih++) {
    unsigned tmp = ih*pngwidth*4;
    for (iw = 0; iw < pngwidth; iw++) {
      bitmap[tmp+0] = image[4*pngwidth*ih+iw*4+2];
      bitmap[tmp+1] = image[4*pngwidth*ih+iw*4+1];
      bitmap[tmp+2] = image[4*pngwidth*ih+iw*4+0];
      bitmap[tmp+3] = image[4*pngwidth*ih+iw*4+3];
      tmp+=4;
    }
  }

  free(image);

  size_t sz = 0;
  auto* data = reinterpret_cast<char*>(zlib_compress(bitmap, sz));

  writei(sz, gameModule); // size
  fwrite(data, 1, sz, gameModule); // data
}

static int module_write_sprites(const ResourceMap& map, FILE *gameModule) {
  // Now we're going to add sprites
  //edbg << es->spriteCount << " Adding Sprites to Game Module: " << flushl;

  //Magic Number
  fwrite("SPR ",4,1,gameModule);

  //Indicate how many
  int sprite_count = map.size();
  fwrite(&sprite_count,4,1,gameModule);

  int sprite_maxid = 0;
  for (auto msg : map) {
    auto spr = static_cast<const Sprite*>(msg);
    if (spr->id() > sprite_maxid)
      sprite_maxid = spr->id();
  }
  fwrite(&sprite_maxid,4,1,gameModule);

  for (auto msg : map) {
    auto spr = static_cast<const Sprite*>(msg);
    writei(spr->id(),gameModule); //id

    // Track how many subimages we're copying
    int subCount = spr->subimages_size();

    int swidth = 0, sheight = 0;
    for (int i = 0; i < subCount; i++)
    {
      if (!swidth and !sheight) {
        swidth =  spr->width();
        sheight = spr->height();
      }
      else if (swidth != spr->width() or sheight != spr->height()) {
        //user << "Subimages of sprite `" << es->sprites[i].name << "' vary in dimensions; do not want." << flushl;
        return 14;
      }
    }
    if (!(swidth and sheight and subCount)) {
      //user << "Subimages of sprite `" << es->sprites[i].name << "' have zero size." << flushl;
      return 14;
    }

    writei(swidth,gameModule);             // width
    writei(sheight,gameModule);            // height
    writei(spr->origin_x(),gameModule);    // xorig
    writei(spr->origin_y(),gameModule);    // yorig
    writei(spr->bbox_top(),gameModule);    // BBox Top
    writei(spr->bbox_bottom(),gameModule); // BBox Bottom
    writei(spr->bbox_left(),gameModule);   // BBox Left
    writei(spr->bbox_right(),gameModule);  // BBox Right
    writei(spr->bbox_mode(),gameModule);   // BBox Mode
    writei(spr->shape(),gameModule);       // Mask shape

    writei(subCount,gameModule); //subimages

    for (int i = 0; i < subCount; i++) {
      const auto& sub = spr->subimages(i);
      writei(swidth * sheight * 4,gameModule); //size when unpacked
      write_image(sub,gameModule);
      writei(0,gameModule);
    }
  }

  //edbg << "Done writing sprites." << flushl;
  return 0;
}

static int module_write_sounds(const ResourceMap& map, FILE *gameModule) {
  // Now we're going to add sounds
  //edbg << es->soundCount << " Sounds:" << flushl;
  for (size_t i = 0; i < map.size(); i++) {
    //edbg << " " << es->sounds[i].name << flushl;
    fflush(stdout);
  }

  //Magic number
  fwrite("SND ",4,1,gameModule);

  //Indicate how many
  int sound_count = map.size();
  fwrite(&sound_count,4,1,gameModule);

  int sound_maxid = 0;
  for (auto msg : map) {
    auto snd = static_cast<const Sound*>(msg);
    if (snd->id() > sound_maxid)
      sound_maxid = snd->id();
  }
  fwrite(&sound_maxid,4,1,gameModule);

  for (auto msg : map) {
    auto snd = static_cast<const Sound*>(msg);

    // Open sound
    FILE *afile = fopen(snd->data().c_str(),"rb");
    if (!afile)
      continue; // no exist/no size/ommitted

    // Buffer sound
    fseek(afile,0,SEEK_END);
    const size_t flen = ftell(afile);
    unsigned char *fdata = new unsigned char[flen];
    fseek(afile,0,SEEK_SET);
    if (fread(fdata,1,flen,afile) != flen)
      puts("WARNING: Resource stream cut short while loading sound data");
    fclose(afile);

    if (!flen) {
      //user << "Sound `" << es->sounds[i].name << "' has no size. It will be omitted from the game." << flushl;
      continue;
    }

    writei(snd->id(), gameModule);      // id
    writei(flen, gameModule);           // Size
    fwrite(fdata, 1, flen, gameModule); // Sound data
  }

  //edbg << "Done writing sounds." << flushl;
  return 0;
}

static int module_write_backgrounds(const ResourceMap& map, FILE *gameModule) {
  // Now we're going to add backgrounds
  //edbg << es->backgroundCount << " Adding Backgrounds to Game Module: " << flushl;

  //Magic Number
  fwrite("BKG ",4,1,gameModule);

  //Indicate how many
  int back_count = map.size();
  fwrite(&back_count,4,1,gameModule);

  int back_maxid = 0;
  for (auto msg : map) {
    auto bkg = static_cast<const Background*>(msg);
    if (bkg->id() > back_maxid)
      back_maxid = bkg->id();
  }
  fwrite(&back_maxid,4,1,gameModule);

  for (auto msg : map) {
    auto bkg = static_cast<const Background*>(msg);

    writei(bkg->id(),gameModule); //id
    writei(bkg->width(),gameModule); // width
    writei(bkg->height(),gameModule); // height

    writei(false,gameModule); // transparent is deprecated
    writei(bkg->smooth_edges(),gameModule);
    writei(bkg->preload(),gameModule);
    writei(bkg->use_as_tileset(),gameModule);
    writei(bkg->tile_width(),gameModule);
    writei(bkg->tile_height(),gameModule);
    writei(bkg->horizontal_offset(),gameModule);
    writei(bkg->vertical_offset(),gameModule);
    writei(bkg->horizontal_spacing(),gameModule);
    writei(bkg->vertical_spacing(),gameModule);

    write_image(bkg->image(),gameModule);
  }

  //edbg << "Done writing backgrounds." << flushl;
  return 0;
}

static int module_write_fonts(const ResourceMap& /*map*/, FILE *gameModule) {
  //TODO: Add fonts

  // Now we're going to add fonts
  //edbg << es->fontCount << " Adding Fonts to Game Module: " << flushl;

  //Magic Number
  fwrite("FNT ",4,1,gameModule);

  //Indicate how many
  int font_count = 0;
  writei(font_count,gameModule);

  // For each included font
  //for (int i = 0; i < font_count; i++) {}

  //edbg << "Done writing fonts." << flushl;
  return 0;
}

static int module_write_paths(const ResourceMap& map, FILE *gameModule) {
  // Now we're going to add paths
  //edbg << es->pathCount << " Adding Paths to Game Module: " << flushl;

  //Magic Number
  fwrite("PTH ",4,1,gameModule);

  //Indicate how many
  int path_count = map.size();
  fwrite(&path_count,4,1,gameModule);

  int path_maxid = 0;
  for (auto msg : map) {
    auto pth = static_cast<const Path*>(msg);
    if (pth->id() > path_maxid)
      path_maxid = pth->id();
  }
  fwrite(&path_maxid,4,1,gameModule);

  for (auto msg : map) {
    auto pth = static_cast<const Path*>(msg);

    writei(pth->id(),gameModule); //id

    writei(pth->smooth(),gameModule);
    writei(pth->closed(),gameModule);
    writei(pth->precision(),gameModule);
    // possibly snapX/Y?

    // Track how many path points we're copying
    int pointCount = pth->points_size();
    writei(pointCount,gameModule);

    for (int i = 0; i < pointCount; i++) {
      const auto& pnt = pth->points(i);
      writei(pnt.x(),gameModule);
      writei(pnt.y(),gameModule);
      writei(pnt.speed(),gameModule);
    }
  }

  //edbg << "Done writing paths." << flushl;
  return 0;
}

static void flatten_resources(TypeMap& typeMap, TreeNode* root) {
  for (int i = 0; i < root->child_size(); i++) {
    buffers::TreeNode* child = root->mutable_child(i);
    if (child->has_folder())
      flatten_resources(typeMap, child);
    else
      typeMap[child->type_case()].push_back(child);
  }
}

int game_write_assets(const Game& game, bool exe, const string& gameFname) {
  TypeMap typeMap;

  // because we still need to write something even when they're empty
  // let's just force them to exist
  std::vector<TypeCase> requiredTypes = { TypeCase::kSprite, TypeCase::kSound, TypeCase::kBackground, TypeCase::kFont, TypeCase::kPath };
  for (auto& type : requiredTypes) {
    // insert it
    typeMap[type];
  }

  flatten_resources(typeMap, const_cast<TreeNode*>(&game.root()));

  FILE *gameModule;
  int resourceblock_start = 0;
  if (exe) {
    gameModule = fopen(gameFname.c_str(),"ab");
    if (!gameModule) {
      //user << "Failed to append resources to the game. Did compile actually succeed?" << flushl;
      //idpr("Failed to add resources.",-1);
      return 12;
    }
    fseek(gameModule,0,SEEK_END); //necessary on Windows for no reason.
    resourceblock_start = ftell(gameModule);
    if (resourceblock_start < 128) {
      //user << "Compiled game is clearly not a working module; cannot continue" << flushl;
      //idpr("Failed to add resources.",-1);
      return 13;
    }
  } else {
    gameModule = fopen(gameFname.c_str(),"wb");
    if (!gameModule) {
      //user << "Failed to write resources to compiler-specified file, `" << resname << "`. Write permissions to valid path?" << flushl;
      //idpr("Failed to write resources.",-1);
      return 12;
    }
  }

  // Start by setting off our location with a DWord of NULLs
  fwrite("\0\0\0",1,4,gameModule);
  //idpr("Adding Sprites",90);
  irrr(module_write_sprites(typeMap[TypeCase::kSprite], gameModule));
  //edbg << "Finalized sprites." << flushl;
  //idpr("Adding Sounds",93);
  irrr(module_write_sounds(typeMap[TypeCase::kSound], gameModule));
  irrr(module_write_backgrounds(typeMap[TypeCase::kBackground], gameModule));
  irrr(module_write_fonts(typeMap[TypeCase::kFont], gameModule));
  irrr(module_write_paths(typeMap[TypeCase::kPath], gameModule));
  // Tell where the resources start
  fwrite("\0\0\0\0res0",8,1,gameModule);
  fwrite(&resourceblock_start,4,1,gameModule);
  // Close the game module; we're done adding resources
  //idpr("Closing game module and running if requested.",99);
  //edbg << "Closing game module and running if requested." << flushl;
  fclose(gameModule);
  return 0;
}
