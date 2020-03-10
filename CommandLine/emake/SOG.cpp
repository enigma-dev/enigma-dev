#include "SOG.hpp"
#include "Main.hpp"
#include "event_reader/event_parser.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <iostream>

bool ReadSOG(const std::string &input_file, Game *game) {
  event_parse_resourcefile();
  map<string, evpair> event_filenames;
  for (evpair e : event_sequence) {
    event_filenames[event_get_function_name(e.first, e.second) + ".edl"] = e;
    if (!e.second && event_is_instance(e.first, e.second)) {
      // For stacked events, allow more than just the 0th instance.
      // Needs to be big enough to accommodate the largest vk_ constant and also
      // provide a reasonable number of alarm events, user events, etc.
      // Using BYTE_MAX gives us both, and plenty of wiggle room.
      for (int i = 1; i < 255; ++i) {
        event_filenames[event_get_function_name(e.first, i) + ".edl"] = e;
      }
    }
  }

  map<evpair, string> events;
  fs::path targetDir(input_file);
  fs::recursive_directory_iterator iter(targetDir);
  for(auto& p : iter) {
    fs::path i = p.path();
    if (is_regular_file(i)) {
      auto find = event_filenames.find(i.filename().string());
      if (find == event_filenames.end())  {
        errorStream << "Error: unrecognized event file " << i << '.' << std::endl;
        errorStream << "Supported events:" << std::endl;
        for (auto st : event_sequence) {
          errorStream << " - " << event_get_function_name(st.first, st.second);
          if (!st.second && event_is_instance(st.first, st.second)) {
            errorStream << ", " << event_get_function_name(st.first, 1);
            errorStream << ", " << event_get_function_name(st.first, 2);
            errorStream << ", " << "...";
          }
          errorStream << std::endl;
        }
        return false;
      } else if (std::ifstream f{i.string()}) {
        evpair eid = find->second;
        std::string code {
          std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
        };
        if (!events.insert(std::make_pair(eid, code)).second) {
          errorStream << "Logic error: Duplicate event " << i << " ("
                      << eid.first << ", " << eid.second
                      << "): your event configuration is broken." << std::endl;
          return false;
        }
      } else {
        errorStream << "Error: Failed to read " << i << "..." << std::endl;
        return false;
      }
    } else {
      errorStream << "Warning: unexpected directory or irregular file " << i
                  << '.' << std::endl;
    }
  }
  if (events.empty()) {
    errorStream << "Error: Failed to read input \"" << input_file << "\". "
                   "Is the game empty?" << std::endl;
    return false;
  }

  game->AddSimpleObject(events);
  game->AddDefaultRoom();
  return true;
}
