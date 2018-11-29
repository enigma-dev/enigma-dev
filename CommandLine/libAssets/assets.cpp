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
#include "bettersystem/bettersystem.h"
#include "OS_Switchboard.h"
#include "lodepng.h"

#include <zlib.h>

#if CURRENT_PLATFORM_ID == OS_WINDOWS
 #include <windows.h>
#else
 #include <unistd.h>
#endif

#include <map>
#include <vector>
#include <string>

using namespace buffers;
using namespace buffers::resources;
using namespace google::protobuf;

using std::string;
using TypeCase = TreeNode::TypeCase;
using ResourceMap = std::vector<const Message*>;
using TypeMap = std::map<TypeCase, std::pair<ResourceMap, int>>;

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

  size_t sz = bitmap_size;
  auto* data = reinterpret_cast<char*>(zlib_compress(bitmap, sz));

  writei(sz, gameModule); // size
  fwrite(data, 1, sz, gameModule); // data
}

static int module_write_sprites(const ResourceMap& map, int sprite_maxid, FILE *gameModule) {
  // Now we're going to add sprites
  //edbg << es->spriteCount << " Adding Sprites to Game Module: " << flushl;

  //Magic Number
  fwrite("SPR ",4,1,gameModule);

  //Indicate how many
  int sprite_count = map.size();
  fwrite(&sprite_count,4,1,gameModule);
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

static int module_write_sounds(const ResourceMap& map, int sound_maxid, FILE *gameModule) {
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

static int module_write_backgrounds(const ResourceMap& map, int back_maxid, FILE *gameModule) {
  // Now we're going to add backgrounds
  //edbg << es->backgroundCount << " Adding Backgrounds to Game Module: " << flushl;

  //Magic Number
  fwrite("BKG ",4,1,gameModule);

  //Indicate how many
  int back_count = map.size();
  fwrite(&back_count,4,1,gameModule);
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

static int module_write_fonts(const ResourceMap& map, int /*font_maxid*/, FILE *gameModule) {
  //TODO: Add fonts

  // Now we're going to add fonts
  //edbg << es->fontCount << " Adding Fonts to Game Module: " << flushl;

  //Magic Number
  fwrite("FNT ",4,1,gameModule);

  //Indicate how many
  int font_count = map.size();
  writei(font_count,gameModule);

  // For each included font
  for (auto msg : map) {
    auto fnt = static_cast<const Font*>(msg);

    int w = 64, h = 64;
    unsigned char *bigtex = new unsigned char[w * h];

    writei(fnt->id(),gameModule);
    writei(w,gameModule), writei(h,gameModule);
    fwrite(bigtex,1,w*h,gameModule);
    fwrite("done",1,4,gameModule);

    delete[] bigtex;

    fwrite("endf",1,4,gameModule);
  }

  //edbg << "Done writing fonts." << flushl;
  return 0;
}

static int module_write_paths(const ResourceMap& map, int path_maxid, FILE *gameModule) {
  // Now we're going to add paths
  //edbg << es->pathCount << " Adding Paths to Game Module: " << flushl;

  //Magic Number
  fwrite("PTH ",4,1,gameModule);

  //Indicate how many
  int path_count = map.size();
  fwrite(&path_count,4,1,gameModule);
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
    if (child->has_folder()) {
      flatten_resources(typeMap, child);
    } else {
      const Descriptor *desc = child->GetDescriptor();
      const Reflection *refl = child->GetReflection();

      const OneofDescriptor *typeOneof =  desc->FindOneofByName("type");
      const FieldDescriptor *typeField = refl->GetOneofFieldDescriptor(*child, typeOneof);
      if (!typeField) continue; // might not be set
      const Message *typeMessage = refl->MutableMessage(child, typeField);
      const google::protobuf::FieldDescriptor *idField = typeMessage->GetDescriptor()->FindFieldByName("id");
      if (!idField) continue; // might not have an id field on this type
      int id = typeMessage->GetReflection()->GetInt32(*typeMessage, idField);

      auto& mapPair = typeMap[child->type_case()];
      mapPair.first.push_back(typeMessage);
      if (id > mapPair.second) mapPair.second = id;
    }
  }
}

int game_write_assets(const Game& game, bool append, const string& gameFname) {
  TypeMap typeMap;

  // because we still need to write something even when they're empty
  // let's just force them to exist
  std::vector<TypeCase> requiredTypes = { TypeCase::kSprite, TypeCase::kSound, TypeCase::kBackground, TypeCase::kFont, TypeCase::kPath };
  for (auto& type : requiredTypes) {
    // insert it
    typeMap[type].second = 0;
  }

  flatten_resources(typeMap, const_cast<TreeNode*>(&game.root()));

  FILE *gameModule;
  int resourceblock_start = 0;
  if (append) {
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
  irrr(module_write_sprites(typeMap[TypeCase::kSprite].first, typeMap[TypeCase::kSprite].second, gameModule));
  //edbg << "Finalized sprites." << flushl;
  //idpr("Adding Sounds",93);
  irrr(module_write_sounds(typeMap[TypeCase::kSound].first, typeMap[TypeCase::kSound].second, gameModule));
  irrr(module_write_backgrounds(typeMap[TypeCase::kBackground].first, typeMap[TypeCase::kBackground].second, gameModule));
  irrr(module_write_fonts(typeMap[TypeCase::kFont].first, typeMap[TypeCase::kFont].second, gameModule));
  irrr(module_write_paths(typeMap[TypeCase::kPath].first, typeMap[TypeCase::kPath].second, gameModule));
  // Tell where the resources start
  fwrite("\0\0\0\0res0",8,1,gameModule);
  fwrite(&resourceblock_start,4,1,gameModule);
  // Close the game module; we're done adding resources
  //idpr("Closing game module and running if requested.",99);
  //edbg << "Closing game module and running if requested." << flushl;
  fclose(gameModule);
  return 0;
}

void game_launch(const char* exe_filename, const char* proj_filename, int mode) {
  // The games working directory, in run/debug it is the GMK/GMX location where the IDE is working with the project,
  // in compile mode it is the same as program_directory, or where the (*.exe executable) is located.
  // The working_directory global is set in the main() of each platform using the platform specific function.
  // This the exact behaviour of GM8.1
  std::vector<char> prevdir(size_t(4096));
  string newdir = (proj_filename != NULL && strlen(proj_filename) > 0) ? string(proj_filename) : string(exe_filename);
  #if CURRENT_PLATFORM_ID == OS_WINDOWS
    if (newdir[0] == '/' || newdir[0] == '\\') {
      newdir = newdir.substr(1, newdir.size());
    }
  #endif
  newdir = newdir.substr( 0, newdir.find_last_of( "\\/" ));

  #if CURRENT_PLATFORM_ID == OS_WINDOWS
  GetCurrentDirectory( 4096, prevdir.data() );
  SetCurrentDirectory(newdir.c_str());
  #else
  getcwd (prevdir.data(), 4096);
  chdir(newdir.c_str());
  #endif

  string rprog = "$game"/*compilerInfo.exe_vars["RUN-PROGRAM"]*/, rparam = "";//compilerInfo.exe_vars["RUN-PARAMS"];
  rprog = string_replace_all(rprog,"$game",exe_filename);
  rparam = string_replace_all(rparam,"$game",exe_filename);
  //user << "Running \"" << rprog << "\" " << rparam << flushl;
  int gameres = e_execs(rprog, rparam);
  //user << "\n\nGame returned " << gameres << "\n";

  // Restore the compilers original working directory.
  #if CURRENT_PLATFORM_ID == OS_WINDOWS
  SetCurrentDirectory(prevdir.data());
  #else
  chdir(prevdir.data());
  #endif
}
