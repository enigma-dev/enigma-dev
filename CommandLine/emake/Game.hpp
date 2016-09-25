#ifndef EMAKE_GAME_HPP
#define EMAKE_GAME_HPP

#include "backend/EnigmaStruct.h"

#include <vector>
#include <string>

class Game
{
public:
  EnigmaStruct* ConstructGame();
  ~Game();

protected:
  EnigmaStruct* _game;
  std::vector<Script*> _scripts;
  std::vector<Sprite*> _sprites;
  std::vector<Shader*> _shaders;
  std::vector<Sound*> _sounds;
  std::vector<Font*> _fonts;
  std::vector<Background*> _backgrounds;
  std::vector<Path*> _paths;
  std::vector<Timeline*> _timelines;
  std::vector<GmObject*> _objects;
  std::vector<Room*> _rooms;
  GameSettings* _gameSettings;
};

#endif
