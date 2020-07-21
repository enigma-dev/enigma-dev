#include "SOG.hpp"
#include "Main.hpp"
#include "event_reader/event_parser.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <iostream>

using std::map;
using std::string;
using evpair = std::pair<int, int>;

bool ReadSOG(const std::string &input_file, Game *game) {
  EventData event_data(ParseEventFile("events.ey"));
  map<evpair, string> events;
  fs::path targetDir(input_file);
  fs::recursive_directory_iterator iter(targetDir);
  for(auto& p : iter) {
    fs::path i = p.path();
    if (is_regular_file(i)) {
      std::string evstr = i.filename().string();
      if (evstr.length() > 4 && evstr.substr(evstr.length() - 4) == ".edl")
        evstr.erase(evstr.length() - 4);
      Event ev = event_data.DecodeEventString(evstr);
      if (ev.IsValid())  {
        if (std::ifstream f{i.string()}) {
          evpair eid = event_data.reverse_get_event(ev);
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
        errorStream << "Error: unrecognized event file " << i << '.' << std::endl;
        errorStream << "Supported events:" << std::endl;
        for (auto st : event_data.events()) {
          errorStream << " - " << st.ExampleIDStrings();
          errorStream << std::endl;
        }
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
