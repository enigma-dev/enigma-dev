#include "SOG.hpp"
#include "Main.hpp"
#include "event_reader/event_parser.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <fstream>
#include <iostream>

using std::map;
using evpair = std::pair<int, int>;

bool ReadSOG(const std::string &input_file, Game *game) {
  EventData event_data(ParseEventFile("events.res"));
  map<evpair, string> events;
  boost::filesystem::path targetDir(input_file);
  boost::filesystem::recursive_directory_iterator iter(targetDir), eod;
  BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)) {
    if (is_regular_file(i)) {
      if (Event ev = event_data.DecodeEventString(i.filename().string()))  {
        if (std::ifstream f{i.string()}) {
          evpair eid = reverse_lookup_legacy_event(ev);
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
