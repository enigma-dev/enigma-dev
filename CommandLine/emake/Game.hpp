#ifndef EMAKE_GAME_HPP
#define EMAKE_GAME_HPP

#include "backend/EnigmaStruct.h"

#include <map>
#include <string>
#include <vector>

class Game
{
public:
  void AddSimpleObject(const std::map<std::pair<int,int>, std::string> &events);
  void AddDefaultRoom();
  EnigmaStruct* ConstructGame();
  void SetOutputFile(std::string fName);

protected:
  EnigmaStruct _game;
  std::string _output_filename;
  std::vector<Script> _scripts;
  std::vector<Sprite> _sprites;
  std::vector<Shader> _shaders;
  std::vector<Sound> _sounds;
  std::vector<Font> _fonts;
  std::vector<Background> _backgrounds;
  std::vector<Path> _paths;
  std::vector<Timeline> _timelines;
  std::vector<GmObject> _objects;
  std::vector<Room> _rooms;

  std::vector<std::vector<MainEvent>> _main_events;
  std::vector<std::vector<Event>> _events;
  std::vector<std::vector<Instance>> _instances;
  std::vector<std::string> _strings;

  const char* SS(std::string str);
  void AddSimpleEvent(std::vector<MainEvent>* main_events,
                      int main_id, int id, std::string code);
};

#endif
