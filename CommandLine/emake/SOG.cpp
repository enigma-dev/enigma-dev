#include "SOG.hpp"
#include "compiler/event_reader/event_parser.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
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
  boost::filesystem::path targetDir(input_file);
  boost::filesystem::recursive_directory_iterator iter(targetDir), eod;
  BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)) {
    if (is_regular_file(i)) {
      auto find = event_filenames.find(i.filename().string());
      if (find == event_filenames.end())  {
        std::cerr << "Error: unrecognized event file " << i << '.' << std::endl;
        std::cerr << "Supported events:" << std::endl;
        for (auto st : event_sequence) {
          std::cerr << " - " << event_get_function_name(st.first, st.second);
          if (!st.second && event_is_instance(st.first, st.second)) {
            std::cerr << ", " << event_get_function_name(st.first, 1);
            std::cerr << ", " << event_get_function_name(st.first, 2);
            std::cerr << ", " << "...";
          }
          std::cerr << std::endl;
        }
        return false;
      } else if (std::ifstream f{i.string()}) {
        evpair eid = find->second;
        std::string code {
          std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
        };
        if (!events.insert(std::make_pair(eid, code)).second) {
          std::cerr << "Logic error: Duplicate event " << i << " ("
                    << eid.first << ", " << eid.second
                    << "): your event configuration is broken." << std::endl;
          return false;
        }
      } else {
        std::cerr << "Error: Failed to read " << i << "..." << std::endl;
        return false;
      }
    } else {
      std::cerr << "Warning: unexpected directory or irregular file " << i
                << '.' << std::endl;
    }
  }
  if (events.empty()) {
    std::cerr << "Error: Failed to read input \"" << input_file << "\". "
                 "Is the game empty?" << std::endl;
    return false;
  }

  game->AddSimpleObject(events);
  game->AddDefaultRoom();
  return true;
}
