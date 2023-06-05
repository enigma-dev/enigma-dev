#include "steam_main.h"

namespace steamworks {
bool steam_main::init() { return SteamAPI_Init(); }

void steam_main::shutdown() { SteamAPI_Shutdown(); }

HSteamPipe steam_main::get_hSteam_pipe() { return SteamAPI_GetHSteamPipe(); }

bool steam_main::restart_app_if_necessary(unsigned int unOwnAppID) { return SteamAPI_RestartAppIfNecessary(unOwnAppID); }
}  // namespace Steamworks
