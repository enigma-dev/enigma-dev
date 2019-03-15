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
  deprecated::JavaStruct::EnigmaStruct* ConstructGame();
  void SetOutputFile(std::string fName);

protected:
  deprecated::JavaStruct::EnigmaStruct _game;
  std::string _output_filename;
  std::vector<deprecated::JavaStruct::Script> _scripts;
  std::vector<deprecated::JavaStruct::Sprite> _sprites;
  std::vector<deprecated::JavaStruct::Shader> _shaders;
  std::vector<deprecated::JavaStruct::Sound> _sounds;
  std::vector<deprecated::JavaStruct::Font> _fonts;
  std::vector<deprecated::JavaStruct::Background> _backgrounds;
  std::vector<deprecated::JavaStruct::Path> _paths;
  std::vector<deprecated::JavaStruct::Timeline> _timelines;
  std::vector<deprecated::JavaStruct::GmObject> _objects;
  std::vector<deprecated::JavaStruct::Room> _rooms;

  std::vector<std::vector<deprecated::JavaStruct::MainEvent>> _main_events;
  std::vector<std::vector<deprecated::JavaStruct::Event>> _events;
  std::vector<std::vector<deprecated::JavaStruct::Instance>> _instances;
  std::vector<std::string> _strings;

  const char* SS(std::string str);
  void AddSimpleEvent(std::vector<deprecated::JavaStruct::MainEvent>* main_events,
                      int main_id, int id, std::string code);
};

#endif // EMAKE_GAME_HPP
