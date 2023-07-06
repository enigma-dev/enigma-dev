#include "steamworks.h"

namespace enigma {

void write_appid_file() {
  std::string filename = fs::temp_directory_path().string();

#ifdef __linux__
  filename += '/';
#endif

  filename += "steam_appid.txt";

  if (fs::exists(filename)) {
    DEBUG_MESSAGE("steam_appid.txt already exists. Ignoring ...", M_INFO);
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
    DEBUG_MESSAGE("Calling steam_init while the API is already initialized. Ignoring ...", M_WARNING);
    return;
  }

  if (!steamworks::c_main::init()) {
    DEBUG_MESSAGE(
        "Calling steam_init failed. Make sure that the Steam client is running. A running Steam client is required "
        "to provide implementations of the various Steamworks interfaces and the Steam client can determine the App "
        "ID of game. If you're running your application from the executable or debugger directly then you must have a "
        "steam_appid.txt in your game directory next to the executable (this is in C:/msys64/tmp/ on Windows and /tmp/ "
        "on Linux), with your app ID in it and nothing else. Steam will look for this file in the current working "
        "directory. If you are running your executable from a different directory you may need to relocate the "
        "steam_appid.txt file.",
        M_ERROR);
  }
}

}  // namespace enigma
