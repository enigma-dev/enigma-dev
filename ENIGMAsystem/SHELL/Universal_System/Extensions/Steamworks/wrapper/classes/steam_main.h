#if !defined(WRAPPER_STEAM_MAIN_H)
#define WRAPPER_STEAM_MAIN_H

#include "public/steam/steam_api_common.h"
#include "public/steam/steam_api.h"

namespace steamworks {
class steam_main {
  public:
  static bool init();

  static void shutdown();

  static HSteamPipe get_hSteam_pipe();

  static bool restart_app_if_necessary(unsigned int unOwnAppID);
};
}  // namespace Steamworks

#endif  // !WRAPPER_STEAM_MAIN_H
