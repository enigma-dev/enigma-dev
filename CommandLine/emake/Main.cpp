#include "OptionsParser.hpp"
#include "EnigmaPlugin.hpp"
#include "Game.hpp"

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

bool read_3fg(const std::string &input_file, Game *game) {
  std::string create, step, draw;
  if (std::ifstream f{input_file + "/create.edl"}) {
    create = {
      std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
    };
  }
  if (std::ifstream f{input_file + "/step.edl"}) {
    step = {
      std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
    };
  }
  if (std::ifstream f{input_file + "/draw.edl"}) {
    draw = {
      std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()
    };
  }
  if (create.empty() && step.empty() && draw.empty()) {
    std::cerr << "Error: Failed to read input \"" << input_file << "\". "
                 "Is the game empty?" << std::endl;
    return false;
  }
  game->AddSimpleObject(create, step, draw);
  game->AddDefaultRoom();
  return true;
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

    bool _run = options.GetOption("run").as<bool>();
    if (!_run) plugin.HandleGameLaunch();

    Game game;
    std::string input_file = options.GetOption("input").as<std::string>();
    if (input_file.size()) {
      if (input_file.size() < 4 || input_file[input_file.length() - 4] != '.') {
        std::cerr << "Error: Unknown filetype: can't read input file \""
                  << input_file << "\"." << std::endl;
        return 1;
      }
      std::string ext = tolower(input_file.substr(input_file.length() - 3, 3));
      if (ext != "3fg") {
        if (ext == "egm") {
          std::cerr << "EGM format not yet supported. "
                       "Please use LateralGM for the time being." << std::endl;
        } else if (ext == "gmx") {
          std::cerr << "GMX format not supported. "
                       "Please use LateralGM or find a converter." << std::endl;
          return 1;
        } else {
          std::cerr << "Error: Unknown filetype: can't read input file \""
                    << input_file << "\"." << std::endl;
        }
        return 1;
      }
      if (!read_3fg(input_file, &game)) return 1;
    } else {
      std::cerr << "Warning: No game file specified. "
                   "Building an empty game." << std::endl;
    }

    std::string output_file = options.GetOption("output").as<std::string>();
    return plugin.BuildGame(game.ConstructGame(), mode, output_file.c_str());
  }

  return result;
}
