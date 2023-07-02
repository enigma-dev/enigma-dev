#include "steamworks.h"

namespace enigma {

void write_appid_file() {
  std::string filename = fs::temp_directory_path().string();

#ifdef __linux__
  filename += '/';
#endif

  filename += "steam_appid.txt";

  if (fs::exists(filename)) {
    DEBUG_MESSAGE("steam_appid.txt already exists. Skipping...", M_INFO);
    return;
  }

  std::ofstream appid_file;

  appid_file.open(filename.c_str());

  if (appid_file.is_open()) {
    appid_file << "480";  // appID for spacewar game which is the default game for steamworks SDK
    appid_file.close();
    DEBUG_MESSAGE("steam_appid.txt created successfully.", M_INFO);
  } else
    DEBUG_MESSAGE("Unable to open steam_appid.txt. Consider creating steam_appid.txt file manually.", M_ERROR);
}

void extension_steamworks_init() {
  write_appid_file();

  if (steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_init but is already initialized.", M_ERROR);
    return;
  }

  if (!steamworks::c_main::init()) {
    DEBUG_MESSAGE(
        "SteamApi_Init returned false. Steam isn't running, couldn't find Steam, App ID is ureleased, Don't own App "
        "ID.",
        M_ERROR);
    return;
  }
}

}  // namespace enigma
