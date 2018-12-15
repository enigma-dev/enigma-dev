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

#include "Proto2ES.h"

#include <unordered_map>
#include <vector>

void AddSprite(const char* name, const buffers::resources::Sprite& spr);
SubImage AddSubImage(const std::string fPath);
void AddSound(const char* name, const buffers::resources::Sound& snd);
void AddBackground(const char* name, const buffers::resources::Background& bkg);
void AddPath(const char* name, const buffers::resources::Path& pth);
PathPoint AddPathPoint(const buffers::resources::Path::Point& pnt);
void AddScript(const char* name, const buffers::resources::Script& scr);
void AddShader(const char* name, const buffers::resources::Shader& shr);
void AddFont(const char* name, const buffers::resources::Font& fnt);
void AddTimeline(const char* name, buffers::resources::Timeline* tml);
void AddObject(const char* name, buffers::resources::Object* obj);
void AddRoom(const char* name, const buffers::resources::Room& rmn);
Instance AddInstance(const buffers::resources::Room::Instance& inst);
Tile AddTile(const buffers::resources::Room::Tile& tile);
View AddView(const buffers::resources::Room::View& view);
BackgroundDef AddRoomBackground(const buffers::resources::Room::Background& bkg);
void AddInclude(const char* name, const buffers::resources::Include& inc);
void WriteSettings(GameSettings &gs, const buffers::resources::Settings& set);

static std::unordered_map<int, int> countMap;
static std::unordered_map<std::string, int> idMap;

static void CacheNames(const buffers::TreeNode& root) {
  using TypeCase = buffers::TreeNode::TypeCase;

  for (auto& message : root.child()) {
    if (message.type_case() == TypeCase::kFolder) {
      CacheNames(message);
      continue;
    }
    countMap[message.type_case()]++;

    const google::protobuf::Descriptor *desc = message.GetDescriptor();
    const google::protobuf::Reflection *refl = message.GetReflection();

    const google::protobuf::OneofDescriptor *typeOneof =  desc->FindOneofByName("type");
    const google::protobuf::FieldDescriptor *typeField = refl->GetOneofFieldDescriptor(message, typeOneof);
    if (!typeField) continue; // might not be set
    const google::protobuf::Message &typeMessage = refl->GetMessage(message, typeField);
    const google::protobuf::FieldDescriptor *idField = typeMessage.GetDescriptor()->FindFieldByName("id");
    if (!idField) continue; // might not have an id field on this type
    idMap[message.name()] = typeMessage.GetReflection()->GetInt32(typeMessage, idField);
  }
}

int Name2Id(const std::string &name) {
  auto it = idMap.find(name);
  return (it != idMap.end()) ? it->second : -1;
}

inline std::string string_replace_all(std::string str, std::string substr, std::string nstr)
{
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos)
  {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

unsigned int BGR2RGBA(unsigned int x) {
  return (((x & 0xFF0000) >> 16) | (x & 0xFF00) | ((x & 0xFF) << 16)) << 8 | 0xFF;
}

std::string Argument2Code(const buffers::resources::Argument& arg) {
  using buffers::resources::ArgumentKind;
  std::string val = arg.string();

  switch (arg.kind()) {
    case ArgumentKind::ARG_BOTH:
      // treat as literal if starts with quote (")
      if (val[0] == '"' || val[0] == '\'') return val;
      // else fall through
    case ArgumentKind::ARG_STRING:
      return '\"' + string_replace_all(string_replace_all(val, "\\", "\\\\"), "\"", "\"+'\"'+\"") + '\"';
    case ArgumentKind::ARG_BOOLEAN:
      return std::to_string(val != "0");
    case ArgumentKind::ARG_SPRITE:
      return arg.has_sprite() ? arg.sprite() : "-1";
    case ArgumentKind::ARG_SOUND:
      return arg.has_sound() ? arg.sound() : "-1";
    case ArgumentKind::ARG_BACKGROUND:
      return arg.has_background() ? arg.background() : "-1";
    case ArgumentKind::ARG_PATH:
      return arg.has_path() ? arg.path() : "-1";
    case ArgumentKind::ARG_SCRIPT:
      return arg.has_script() ? arg.script() : "-1";
    case ArgumentKind::ARG_OBJECT:
      return arg.has_object() ? arg.object() : "-1";
    case ArgumentKind::ARG_ROOM:
      return arg.has_room() ? arg.room() : "-1";
    case ArgumentKind::ARG_FONT:
      return arg.has_font() ? arg.font() : "-1";
    case ArgumentKind::ARG_TIMELINE:
      return arg.has_timeline() ? arg.timeline() : "-1";
    case ArgumentKind::ARG_MENU:
    case ArgumentKind::ARG_COLOR:
    default:
      if (val.empty()) {
        if (arg.kind() == ArgumentKind::ARG_STRING)
          return "\"\"";
        else
          return "0";
      }
      // else fall all the way through
  }

  return val;
}

std::string Actions2Code(const ::google::protobuf::RepeatedPtrField< buffers::resources::Action >& actions) {
  using buffers::resources::ActionKind;
  using buffers::resources::ActionExecution;
  std::string code = "";

  int numberOfBraces = 0; // gm ignores brace actions which are in the wrong place or missing
  int numberOfIfs = 0; // gm allows multipe else actions after 1 if, so its important to track the number

  for (const auto &action : actions) {
    const auto &args = action.arguments();

    bool in_with = action.use_apply_to() && action.who_name() != "self";
    if (in_with)
      code += "with (" + action.who_name() + ")\n";

    switch (action.kind()) {
      case ActionKind::ACT_BEGIN:
        code += '{';
        numberOfBraces++;
        break;
      case ActionKind::ACT_END:
        if (numberOfBraces > 0) {
          code += '}';
          numberOfBraces--;
        }
        break;
      case ActionKind::ACT_ELSE:
        if (numberOfIfs > 0) {
          code += "else ";
          numberOfIfs--;
        }
        break;
      case ActionKind::ACT_EXIT:
        code += "exit;";
        break;
      case ActionKind::ACT_REPEAT:
        code += "repeat (" + args.Get(0).string() + ")";
        break;
      case ActionKind::ACT_VARIABLE:
        code += args.Get(0).string();
        if (action.relative())
          code += " += ";
        else
          code += " = ";
        code += args.Get(1).string();
        break;
      case ActionKind::ACT_CODE:
        code += "{\n" + args.Get(0).string() + "\n/**/\n}";
        break;
      case ActionKind::ACT_NORMAL:
        if (action.exe_type() == ActionExecution::EXEC_NONE) break;

        if (action.is_question()) {
          code += "__if__ = ";
          numberOfIfs++;
        }

        if (action.is_not())
          code += "!";

        if (action.relative()) {
          if (action.is_question())
            code += "(argument_relative = " + std::to_string(action.relative()) + ", ";
          else
            code += "{\nargument_relative = " + std::to_string(action.relative()) + ";\n";
        }

        if (action.is_question() && action.exe_type() == ActionExecution::EXEC_CODE)
          code += action.code_string();
        else
          code += action.function_name();

        if (action.exe_type() == ActionExecution::EXEC_FUNCTION) {
          code += '(';
          for (int i = 0; i < args.size(); i++) {
            if (i != 0)
              code += ',';
            code += Argument2Code(args.Get(i));
          }
          code += ')';
        }

        if (action.relative())
          code += action.is_question() ? ");" : "\n}";
        if (action.is_question()) {
          code += "\nif (__if__)";
        }
        break;
      default:
        break;
    }
    code += '\n';
  }

  // someone forgot the closing block action
  if (numberOfBraces > 0)
    for (int i = 0; i < numberOfBraces; i++)
      code += "\n}";

  if (numberOfIfs > 0)
    code = "var __if__ = false;\n" + code;

  return code;
}

#include <zlib.h>

unsigned char* zlib_compress(unsigned char* inbuffer,int &actualsize)
{
    uLongf outsize=(int)(actualsize*1.1)+12;
    Bytef* outbytef=new Bytef[outsize];

    compress(outbytef,&outsize,(Bytef*)inbuffer,actualsize);

    actualsize = outsize;

    return (unsigned char*)outbytef;
}

#include "lodepng.h"

Image AddImage(const std::string fname) {
  Image i = Image();

  unsigned error;
  unsigned char* image;
  unsigned pngwidth, pngheight;

  error = lodepng_decode32_file(&image, &pngwidth, &pngheight, fname.c_str());
  if (error)
  {
    printf("error %u: %s\n", error, lodepng_error_text(error));
    return i;
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
  i.width  = pngwidth;
  i.height = pngheight;
  i.dataSize = bitmap_size;
  i.data = reinterpret_cast<char*>(zlib_compress(bitmap, i.dataSize));

  return i;
}

void AddResource(buffers::Game* protobuf, buffers::TreeNode* node) {
  for (int i = 0; i < node->child_size(); i++) {
    buffers::TreeNode* child = node->mutable_child(i);
    const char* name = child->name().c_str();
    if (child->has_folder())
      AddResource(protobuf, child);
    if (child->has_sprite())
      AddSprite(name, child->sprite());
    if (child->has_sound())
      AddSound(name, child->sound());
    if (child->has_background())
      AddBackground(name, child->background());
    if (child->has_path())
      AddPath(name, child->path());
    if (child->has_script())
      AddScript(name, child->script());
    if (child->has_shader())
      AddShader(name, child->shader());
    if (child->has_font())
      AddFont(name, child->font());
    if (child->has_timeline())
      AddTimeline(name, child->mutable_timeline());
    if (child->has_object())
      AddObject(name, child->mutable_object());
    if (child->has_room())
      AddRoom(name, child->room());
    if (child->has_include())
      AddInclude(name, child->include());
  }
}

static Sprite* sprites;
static Sound* sounds;
static Background* backgrounds;
static Path* paths;
static Script* scripts;
static Shader* shaders;
static Font* fonts;
static Timeline* timelines;
static GmObject* objects;
static Room* rooms;
static Include* includes;

static size_t sprite, sound, background, path, script, shader, font, timeline, object, room, include;

template<typename T>
T* AllocateGroup(T** group, int &count, buffers::TreeNode::TypeCase typeCase) {
  count = countMap[typeCase];
  return (*group = new T[count]);
}

EnigmaStruct* Proto2ES(buffers::Game* protobuf) {
  using TypeCase = buffers::TreeNode::TypeCase;

  idMap.clear();
  countMap.clear();
  sprite = 0, sound = 0, background = 0, path = 0, script = 0, shader = 0, font = 0, timeline = 0, object = 0, room = 0, include = 0;

  EnigmaStruct *es = new EnigmaStruct();

  es->gameSettings.gameIcon = "";
  es->gameSettings.letEscEndGame = true;
  es->gameSettings.letF4SwitchFullscreen = true;
  es->gameSettings.treatCloseAsEscape = true;
  es->gameSettings.alwaysOnTop = true;
  es->gameSettings.gameId = 0;
  es->gameSettings.company = "";
  es->gameSettings.description = "";
  es->gameSettings.version = "";
  es->gameSettings.product = "";
  es->gameSettings.version = "";
  es->gameSettings.copyright = "";
  es->gameInfo.gameInfoStr = "";
  es->gameInfo.formCaption = "";

  auto root = protobuf->mutable_root();
  CacheNames(*root);

  es->sprites = AllocateGroup(&sprites, es->spriteCount, TypeCase::kSprite);
  es->sounds = AllocateGroup(&sounds, es->soundCount, TypeCase::kSound);
  es->backgrounds = AllocateGroup(&backgrounds, es->backgroundCount, TypeCase::kBackground);
  es->paths = AllocateGroup(&paths, es->pathCount, TypeCase::kPath);
  es->scripts = AllocateGroup(&scripts, es->scriptCount, TypeCase::kScript);
  es->shaders = AllocateGroup(&shaders, es->shaderCount, TypeCase::kShader);
  es->fonts = AllocateGroup(&fonts, es->fontCount, TypeCase::kFont);
  es->timelines = AllocateGroup(&timelines, es->timelineCount, TypeCase::kTimeline);
  es->gmObjects = AllocateGroup(&objects, es->gmObjectCount, TypeCase::kObject);
  es->rooms = AllocateGroup(&rooms, es->roomCount, TypeCase::kRoom);
  es->includes = AllocateGroup(&includes, es->includeCount, TypeCase::kInclude);

  AddResource(protobuf, root);

  return es;
}

void AddSprite(const char* name, const buffers::resources::Sprite& spr) {
  Sprite& s = sprites[sprite++];

  s.name = name;
  s.id = spr.id();

  s.transparent = false;
  s.shape = spr.shape();
  s.alphaTolerance = spr.alpha_tolerance();
  s.separateMask = spr.separate_mask();
  s.smoothEdges = spr.smooth_edges();
  s.preload = spr.preload();
  s.originX = spr.origin_x();
  s.originY = spr.origin_y();

  s.bbMode = spr.bbox_mode();
  s.bbLeft = spr.bbox_left();
  s.bbRight = spr.bbox_right();
  s.bbTop = spr.bbox_top();
  s.bbBottom = spr.bbox_bottom();

  s.subImageCount = spr.subimages_size();
  if (s.subImageCount > 0) {
    s.subImages = new SubImage[s.subImageCount];
    for (int i = 0; i < s.subImageCount; ++i) {
      s.subImages[i].image = AddImage(spr.subimages(i));
    }
  }

  s.maskShapeCount = 0;
  s.maskShapes = nullptr;
}

void AddSound(const char* name, const buffers::resources::Sound& snd) {
  Sound& s = sounds[sound++];

  s.name = name;
  s.id = snd.id();

  s.kind = snd.kind();
  s.fileType = snd.file_extension().c_str();
  s.fileName = snd.file_name().c_str();
  s.volume = snd.volume();
  s.pan = snd.pan();
  s.preload = snd.preload();

  // Open sound
  FILE *afile = fopen(snd.data().c_str(),"rb");
  if (!afile)
    return;

  // Buffer sound
  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  unsigned char *fdata = new unsigned char[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");
  fclose(afile);

  s.data = fdata;
  s.size = flen;
}

void AddBackground(const char* name, const buffers::resources::Background& bkg) {
  Background& b = backgrounds[background++];

  b.name = name;
  b.id = bkg.id();

  b.transparent = false;
  b.smoothEdges = bkg.smooth_edges();
  b.preload = bkg.preload();
  b.useAsTileset = bkg.use_as_tileset();

  b.tileWidth = bkg.tile_width();
  b.tileHeight = bkg.tile_height();
  b.hOffset = bkg.horizontal_offset();
  b.vOffset = bkg.vertical_offset();
  b.hSep = bkg.horizontal_spacing();
  b.vSep = bkg.vertical_spacing();

  b.backgroundImage = AddImage(bkg.image());
}

void AddPath(const char* name, const buffers::resources::Path& pth) {
  Path& p = paths[path++];

  p.name = name;
  p.id = pth.id();

  p.smooth = pth.smooth();
  p.closed = pth.closed();
  p.precision = pth.precision();

  p.snapX = pth.hsnap();
  p.snapY = pth.vsnap();

  p.pointCount = pth.points_size();
  if (p.pointCount > 0) {
    p.points = new PathPoint[p.pointCount];
    for (int i=0; i < p.pointCount; ++i) {
      p.points[i] = AddPathPoint(pth.points(i));
    }
  }
}

PathPoint AddPathPoint(const buffers::resources::Path::Point& pnt) {
  PathPoint p = PathPoint();

  p.x = pnt.x();
  p.y = pnt.y();
  p.speed = pnt.speed();

  return p;
}

void AddScript(const char* name, const buffers::resources::Script& scr) {
  Script& s = scripts[script++];

  s.name = name;
  s.id = scr.id();

  s.code = scr.code().c_str();
}

void AddShader(const char* name, const buffers::resources::Shader& shr) {
  Shader& s = shaders[shader++];

  s.name = name;
  s.id = shr.id();

  s.vertex = shr.vertex_code().c_str();
  s.fragment = shr.fragment_code().c_str();
  s.type = shr.type().c_str();
  s.precompile = shr.precompile();
}

void AddFont(const char* name, const buffers::resources::Font& fnt) {
  Font& f = fonts[font++];

  f.name = name;
  f.id = fnt.id();

  f.fontName = fnt.font_name().c_str();
  f.size = fnt.size();
  f.bold = fnt.bold();
  f.italic = fnt.italic();

  f.glyphRangeCount = 0;//fnt.ranges_size();
  if (f.glyphRangeCount > 0) {
    f.glyphRanges = new GlyphRange[f.glyphRangeCount];
    for (int i=0; i < f.glyphRangeCount; ++i) {
      f.glyphRanges[i].rangeMin = 0;
      f.glyphRanges[i].rangeMax = 0;
      f.glyphRanges[i].glyphs = nullptr;
    }
  }
}

void AddTimeline(const char* name, buffers::resources::Timeline* tml) {
  Timeline& t = timelines[timeline++];

  t.name = name;
  t.id = tml->id();

  t.momentCount = tml->moments_size();
  if (t.momentCount > 0) {
    t.moments = new Moment[t.momentCount];
    for (int i = 0; i < t.momentCount; ++i) {
      buffers::resources::Timeline::Moment* mmt = tml->mutable_moments(i);
      t.moments[i].stepNo = mmt->step();
      if (mmt->actions_size() > 0)
        mmt->set_code(Actions2Code(mmt->actions()));
      t.moments[i].code = mmt->code().c_str();
    }
  }
}

void AddObject(const char* name, buffers::resources::Object* obj) {
  GmObject& o = objects[object++];

  o.name = name;
  o.id = obj->id();

  o.spriteId = Name2Id(obj->sprite_name());
  o.solid = obj->solid();
  o.visible = obj->visible();
  o.depth = obj->depth();
  o.persistent = obj->persistent();
  o.parentId = Name2Id(obj->parent_name());
  o.maskId = Name2Id(obj->mask_name());

  std::unordered_map<int,std::vector<Event> > mainEventMap;

  for (int i = 0; i < obj->events_size(); ++i) {
    auto *evt = obj->mutable_events(i);
    std::vector<Event>& events = mainEventMap[evt->type()];
    Event e;
    e.id = evt->has_name() ? Name2Id(evt->name()) : evt->number();
    if (evt->actions_size() > 0)
      evt->set_code(Actions2Code(evt->actions()));
    e.code = evt->code().c_str();
    events.push_back(e);
  }

  o.mainEventCount = mainEventMap.size();
  if (o.mainEventCount > 0) {
    o.mainEvents = new MainEvent[o.mainEventCount];
    int m = 0;
    for (const auto &mainEvent : mainEventMap) {
      MainEvent me = MainEvent();
      me.id = mainEvent.first;
      me.eventCount = mainEvent.second.size();
      if (me.eventCount > 0) {
        me.events = new Event[me.eventCount];
        for (int i = 0; i < me.eventCount; ++i) {
          me.events[i] = mainEvent.second[i];
        }
      }
      o.mainEvents[m++] = me;
    }
  }
}

void AddRoom(const char* name, const buffers::resources::Room& rm) {
  Room& r = rooms[room++];

  r.name = name;
  r.id = rm.id();

  r.caption = rm.caption().c_str();
  r.width = rm.width();
  r.height = rm.height();
  r.speed = rm.speed();
  r.persistent = rm.persistent();
  r.backgroundColor = BGR2RGBA(rm.color());
  r.drawBackgroundColor = rm.show_color();
  r.creationCode = rm.creation_code().c_str();
  r.enableViews = rm.enable_views();

  r.backgroundDefCount = rm.backgrounds_size();
  if (r.backgroundDefCount > 0) {
    r.backgroundDefs = new BackgroundDef[r.backgroundDefCount];
    for (int i = 0; i < r.backgroundDefCount; ++i) {
      r.backgroundDefs[i] = AddRoomBackground(rm.backgrounds(i));
    }
  }

  r.viewCount = rm.views_size();
  if (r.viewCount > 0) {
    r.views = new View[r.viewCount];
    for (int i = 0; i < r.viewCount; ++i) {
      r.views[i] = AddView(rm.views(i));
    }
  }

  r.instanceCount = rm.instances_size();
  if (r.instanceCount > 0) {
    r.instances = new Instance[r.instanceCount];
    for (int i = 0; i < r.instanceCount; ++i) {
      r.instances[i] = AddInstance(rm.instances(i));
    }
  }

  r.tileCount = rm.tiles_size();
  if (r.tileCount > 0) {
    r.tiles = new Tile[r.tileCount];
    for (int i = 0; i < r.tileCount; ++i) {
      r.tiles[i] = AddTile(rm.tiles(i));
    }
  }
}

Instance AddInstance(const buffers::resources::Room::Instance& inst) {
  Instance i = Instance();

  i.id = inst.id();
  i.objectId = Name2Id(inst.object_type());
  i.x = inst.x();
  i.y = inst.y();
  i.locked = inst.editor_settings().locked();
  i.creationCode = inst.creation_code().c_str();
  i.preCreationCode = "";

  return i;
}

Tile AddTile(const buffers::resources::Room::Tile& tile) {
  Tile t = Tile();

  t.id = tile.id();
  t.backgroundId = Name2Id(tile.background_name());
  t.roomX = tile.x();
  t.roomY = tile.y();
  t.locked = tile.editor_settings().locked();
  t.bgX = tile.xoffset();
  t.bgY = tile.yoffset();
  t.width = tile.width();
  t.height = tile.height();
  t.depth = tile.depth();

  return t;
}

View AddView(const buffers::resources::Room::View& view) {
  View v = View();

  v.visible = view.visible();
  v.viewX = view.xview();
  v.viewY = view.yview();
  v.viewW = view.wview();
  v.viewH = view.hview();
  v.portX = view.xport();
  v.portY = view.yport();
  v.portW = view.wport();
  v.portH = view.hport();
  v.borderH = view.hborder();
  v.borderV = view.vborder();
  v.speedH = view.hspeed();
  v.speedV = view.vspeed();
  v.objectId = Name2Id(view.object_following());

  return v;
}

BackgroundDef AddRoomBackground(const buffers::resources::Room::Background& bkg) {
  BackgroundDef b = BackgroundDef();

  b.visible = bkg.visible();
  b.foreground = bkg.foreground();
  b.x = bkg.x();
  b.y = bkg.y();
  b.tileHoriz = bkg.htiled();
  b.tileVert = bkg.vtiled();
  b.hSpeed = bkg.hspeed();
  b.vSpeed = bkg.vspeed();
  b.stretch = bkg.stretch();
  b.backgroundId = Name2Id(bkg.background_name());

  return b;
}

void AddInclude(const char* /*name*/, const buffers::resources::Include& inc) {
  Include& i = includes[include++];

  i.filepath = inc.file_name().c_str();
}

void WriteSettings(GameSettings &gs, const buffers::resources::Settings& set) {
  // General
  const buffers::resources::General &gen = set.general();
  gs.gameId = gen.game_id();
  gs.versionMajor = gen.version_major();
  gs.versionMinor = gen.version_minor();
  gs.versionRelease = gen.version_release();
  gs.versionBuild = gen.version_build();
  gs.company = gen.company().c_str();
  gs.product = gen.product().c_str();
  gs.copyright = gen.copyright().c_str();
  gs.description = gen.description().c_str();
  gs.displayCursor = gen.show_cursor();
  gs.gameIcon = gen.game_icon().c_str();

  // Graphics
  const buffers::resources::Graphics &gfx = set.graphics();
  gs.colorOutsideRoom = gfx.color_outside_room_region();
  gs.letF4SwitchFullscreen = gfx.allow_fullscreen_change();
  gs.interpolate = gfx.interpolate_textures();
  gs.forceSoftwareVertexProcessing = gfx.force_software_vertex_processing();
  gs.useSynchronization = gfx.use_synchronization();
  gs.scaling = gfx.view_scale();

  // Windowing
  const buffers::resources::Windowing &win = set.windowing();
  gs.startFullscreen = win.start_in_fullscreen();
  gs.freezeOnLoseFocus = win.freeze_on_lose_focus();
  gs.allowWindowResize = win.is_sizeable();
  gs.dontDrawBorder = !win.show_border();
  gs.dontShowButtons = !win.show_icons();
  gs.alwaysOnTop = win.stay_on_top();

  // Project Info
  const buffers::resources::Info &inf = set.info();
  gs.author = inf.author_name().c_str();
  gs.version = inf.version().c_str();
  gs.lastChanged = inf.last_changed();
  gs.information = inf.information().c_str();


  //TODO: do something, anything
  gs.letEscEndGame = true;
  gs.treatCloseAsEscape = true;
}
