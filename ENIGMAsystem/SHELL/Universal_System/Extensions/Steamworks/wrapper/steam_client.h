#if !defined(STEAM_CLIENT_H)
#define STEAM_CLIENT_H

#include "wrapper/classes/steam_main.h"

#include <stdexcept>

namespace Steamworks {
class SteamClient {
 private:
  static bool initialized_;

 public:

  /// <summary>
	/// Initialize the steam client.
	/// If <paramref name="asyncCallbacks"/> is false you need to call <see cref="RunCallbacks"/> manually every frame.
	/// </summary>
  static void init(unsigned int appid);
  
  static bool is_valid();

};
}  // namespace Steamworks

#endif  // !STEAM_CLIENT_H
