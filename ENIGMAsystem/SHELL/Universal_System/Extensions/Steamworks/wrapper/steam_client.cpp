#include "steam_client.h"

namespace Steamworks {

void SteamClient::init(unsigned int appid) {
  if (SteamClient::initialized_) throw std::exception("Calling SteamClient::init but is already initialized");

  if (!SteamMain::init()) {
    throw std::exception(
        "SteamApi_Init returned false. Steam isn't running, couldn't find Steam, App ID is ureleased, Don't own App "
        "ID.");
  }

  SteamClient::initialized_ = true;
}

/// <summary>
/// Check if Steam is loaded and accessible.
/// </summary>
bool SteamClient::is_valid() { return SteamClient::initialized_; }

}  // namespace Steamworks
