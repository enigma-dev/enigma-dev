#ifndef STEAMWORKS_H
#define STEAMWORKS_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "game_client/c_main.h"

#include <fstream>
#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;

namespace enigma {

/*
    NOTE:   This function is automatically called by the extension system.
    
    This function initialises the steam APIs. Calls c_main::init().
*/
void extension_steamworks_init();

}  // namespace enigma

#endif  // STEAMWORKS_H
