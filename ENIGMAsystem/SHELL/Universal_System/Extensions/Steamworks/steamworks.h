#ifndef STEAMWORKS_H
#define STEAMWORKS_H

#include "game_client/c_main.h"
// #include "Universal_System/Extensions/DataStructures/include.h"

#include <fstream>
#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;

namespace enigma {

void extension_steamworks_init();

}  // namespace enigma

namespace enigma_user {

// extern unsigned async_load;

// void fireSteamworksEvent();

}  // namespace enigma_user

#endif  // STEAMWORKS_H
