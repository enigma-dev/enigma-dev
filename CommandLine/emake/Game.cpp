#include "Game.hpp"

EnigmaStruct* Game::ConstructGame()
{
    _game = new EnigmaStruct();
    _game->gameSettings.gameIcon = "";
    _game->gameSettings.letEscEndGame = true;
    _game->gameSettings.treatCloseAsEscape = true;
    _game->gameSettings.alwaysOnTop = true;
    _game->gameSettings.gameId = 0;
    _game->gameSettings.company = "";
    _game->gameSettings.description = "";
    _game->gameSettings.version = "";
    _game->gameSettings.product = "";
    _game->gameSettings.version = "";
    _game->gameSettings.copyright = "";
    _game->gameInfo.gameInfoStr = "";
    _game->gameInfo.formCaption = "";
    _game->filename = "game";

    return _game;
}

Game::~Game()
{
    delete _game;
}
