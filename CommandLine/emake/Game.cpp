#include "Game.hpp"

using std::map;
using std::pair;
using std::string;

namespace {

using namespace deprecated::JavaStruct;
constexpr int NO_SPRITE = -1;
constexpr int NO_OBJECT = -1;
constexpr int INSTANCE_ID_MIN = 100001;

}

const char *Game::SS(string str) {
  _strings.emplace_back(std::move(str));
  return _strings.back().c_str();
}

void Game::AddSimpleEvent(std::vector<MainEvent>* main_events,
                          int main_id, int id, string code) {
  _events.push_back({});
  auto &events = _events.back();
  events.push_back({id, SS(code)});

  main_events->push_back({main_id, events.data(), (int) events.size()});
}

void Game::AddSimpleObject(const map<pair<int,int>, string> &events) {
  const int object_index = _objects.size();

  _main_events.push_back({});
  auto &main_events = _main_events.back();

  for (auto event : events) {
    AddSimpleEvent(&main_events,
                   event.first.first, event.first.second,  // Main ID, ID
                   event.second);  // Code
  }

  _objects.push_back({});
  GmObject &object = _objects.back();
  object.name = SS("object" + std::to_string(object_index));
  object.id = object_index;
  object.spriteId = NO_SPRITE;
  object.solid = false;
  object.visible = true;
  object.depth = 0;
  object.persistent = false;
  object.parentId = NO_OBJECT;
  object.maskId = NO_SPRITE;

  object.mainEvents = main_events.data();
  object.mainEventCount = main_events.size();
}

void Game::AddDefaultRoom() {
  if (_rooms.size()) return;

  _rooms.push_back({});
  Room &room = _rooms.back();

  _instances.push_back({});
  auto &instances = _instances.back();

  int instance_id = INSTANCE_ID_MIN;
  for (size_t i = 0; i < _objects.size(); ++i) {
    instances.push_back({
      /* x: */ 0, /* y: */ 0,
      /* objectId: */ _objects[i].id,
      /* id: */ instance_id++,
      /* creationCode: */ "",
      /* preCreationCode: */ "",
      /* locked: */ false
    });
  }

  room.name = "room0";
  room.id = 0;
  room.caption = "";
  room.width = 640;
  room.height = 480;
  room.speed = 30;
  room.persistent = false;
  room.backgroundColor = 0x40C0FFFF;
  room.drawBackgroundColor = true;
  room.creationCode = "";

  room.backgroundDefs = nullptr;
  room.backgroundDefCount = 0;
  room.views = nullptr;
  room.viewCount = 0;

  room.instances = instances.data();
  room.instanceCount = instances.size();
  room.tiles = nullptr;
  room.tileCount = 0;
}

EnigmaStruct* Game::ConstructGame()
{
  _game = {};

  _game.gameSettings.gameIcon = "";
  _game.gameSettings.letEscEndGame = true;
  _game.gameSettings.letF4SwitchFullscreen = true;
  _game.gameSettings.treatCloseAsEscape = true;
  _game.gameSettings.alwaysOnTop = true;
  _game.gameSettings.gameId = 0;
  _game.gameSettings.company = "";
  _game.gameSettings.description = "";
  _game.gameSettings.version = "";
  _game.gameSettings.product = "";
  _game.gameSettings.version = "";
  _game.gameSettings.copyright = "";
  _game.gameInfo.gameInfoStr = "";
  _game.gameInfo.formCaption = "";
  _game.filename = _output_filename.c_str();

  _game.sprites = _sprites.data();
  _game.spriteCount = _sprites.size();
  _game.sounds = _sounds.data();
  _game.soundCount = _sounds.size();
  _game.backgrounds = _backgrounds.data();
  _game.backgroundCount = _backgrounds.size();
  _game.paths = _paths.data();
  _game.pathCount = _paths.size();
  _game.scripts = _scripts.data();
  _game.scriptCount = _scripts.size();
  _game.shaders = _shaders.data();
  _game.shaderCount = _shaders.size();
  _game.fonts = _fonts.data();
  _game.fontCount = _fonts.size();
  _game.timelines = _timelines.data();
  _game.timelineCount = _timelines.size();
  _game.gmObjects = _objects.data();
  _game.gmObjectCount = _objects.size();
  _game.rooms = _rooms.data();
  _game.roomCount = _rooms.size();

  return &_game;
}

void Game::SetOutputFile(std::string fName) {
  _output_filename = fName;
}
