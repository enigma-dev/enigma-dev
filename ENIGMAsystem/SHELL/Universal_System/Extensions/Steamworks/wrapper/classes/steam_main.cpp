#include "steam_main.h"

namespace Steamworks {
bool SteamMain::init() { return SteamAPI_Init(); }

void SteamMain::shutdown() { SteamAPI_Shutdown(); }

HSteamPipe SteamMain::get_hSteam_pipe() { return SteamAPI_GetHSteamPipe(); }

bool SteamMain::restart_app_if_necessary(unsigned int unOwnAppID) { return SteamAPI_RestartAppIfNecessary(unOwnAppID); }
}  // namespace Steamworks
