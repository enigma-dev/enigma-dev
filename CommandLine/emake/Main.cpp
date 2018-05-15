#include "OptionsParser.hpp"
#include "EnigmaPlugin.hpp"
#include "Game.hpp"
#include "SOG.hpp"

#include <fstream>
#include <iostream>
#include <streambuf>

static std::string tolower(const std::string &str) {
  std::string res = str;
  for (size_t i = 0; i < res.length(); ++i) {
    if (res[i] >= 'A' && res[i] <= 'Z') res[i] += 'a' - 'A';
  }
  return res;
}

int main(int argc, char* argv[])
{
  OptionsParser options;
  options.ReadArgs(argc, argv);
  int result = options.HandleArgs();

  if (result != OPTIONS_ERROR && result != OPTIONS_HELP)
  {
    EnigmaPlugin plugin;
    plugin.Init();
    plugin.LogMakeToConsole();
    plugin.SetDefinitions(options.APIyaml().c_str());

    GameMode mode;
    std::string _mode = options.GetOption("mode").as<std::string>();
    
    std::string output_file; 
    
    if (!options.GetOption("output").empty())
      output_file = options.GetOption("output").as<std::string>();
    
    bool list = options.GetOption("list").as<bool>();
    if (list) {
      plugin.PrintBuiltins(output_file);
      return result;
    }

    if (_mode == "Compile")
      mode = emode_compile;
    else if (_mode == "Run")
      mode = emode_run;
    else if (_mode == "Debug")
      mode = emode_debug;
    else if (_mode == "Design")
      mode = emode_design;
    else if (_mode == "Rebuild")
      mode = emode_rebuild;
    else
      mode = emode_invalid;
      
    if (mode == emode_invalid) {
      std::cerr << "Invalid game mode: " << _mode << " aborting!" << std::endl;
      return OPTIONS_ERROR;
    }

    bool _run = options.GetOption("run").as<bool>();
    if (!_run) plugin.HandleGameLaunch();

    Game game;
    std::string input_file = options.GetOption("input").as<std::string>();
    
    // Working directory hacks
    if (mode != emode_compile)
      game.SetOutputFile(input_file);
    
    if (input_file.size()) {
      std::string ext;
      size_t dot = input_file.find_last_of('.');
      if (dot != std::string::npos) ext = tolower(input_file.substr(dot + 1));
      if (ext != "sog") {
        if (ext == "egm") {
          std::cerr << "EGM format not yet supported. "
                       "Please use LateralGM for the time being." << std::endl;
        } else if (ext == "gmx") {
          std::cerr << "GMX format not supported. "
                       "Please use LateralGM or find a converter." << std::endl;
          return 1;
        } else if (ext.empty()) {
          std::cerr << "Error: Unknown filetype: cannot determine type of file "
                    << '"' << input_file << "\"." << std::endl;
        } else {
          std::cerr << "Error: Unknown filetype \"" << ext
                    << "\": cannot read input file \"" << input_file
                    << "\"." << std::endl;
        }
        return 1;
      }
      if (!ReadSOG(input_file, &game)) return 1;
    } else {
      std::cerr << "Warning: No game file specified. "
                   "Building an empty game." << std::endl;
    }

    return plugin.BuildGame(game.ConstructGame(), mode, output_file.c_str());
  }

  return result;
}
