#include "Main.hpp"
#include "OptionsParser.hpp"
#include "EnigmaPlugin.hpp"
#include "Game.hpp"

#ifdef CLI_ENABLE_SERVER
#include "Server.hpp"
#endif

#include "SOG.hpp"

#ifdef CLI_ENABLE_EGM
#include "gmk.h"
#include "gmx.h"
#include "yyp.h"
#endif

#include <boost/filesystem.hpp>

#include <fstream>
#include <iostream>
#include <streambuf>

std::ostream outputStream(std::cout.rdbuf());
std::ostream errorStream(std::cerr.rdbuf());

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

  if (result == OPTIONS_ERROR || result == OPTIONS_HELP)
    return result;

  EnigmaPlugin plugin;
  plugin.Load();
  bool quiet = options.GetOption("quiet").as<bool>();
  if (!quiet) {
    plugin.LogMakeToConsole();
  } else {
    outputStream.rdbuf(nullptr);
    errorStream.rdbuf(nullptr);
  }
#ifdef CLI_ENABLE_EGM
  yyp::bind_output_streams(outputStream, errorStream);
  gmx::bind_output_streams(outputStream, errorStream);
  gmk::bind_output_streams(outputStream, errorStream);
#endif
  CallBack ecb;
  plugin.Init(&ecb);
  plugin.SetDefinitions(options.APIyaml().c_str());

  std::string output_file;

  if (!options.GetOption("output").empty())
    output_file = options.GetOption("output").as<std::string>();

  if (options.HasOption("list")) {
    plugin.PrintBuiltins(output_file);
    return result;
  }

  bool run = options.GetOption("run").as<bool>();
  if (!run) plugin.HandleGameLaunch();

#ifdef CLI_ENABLE_SERVER
  bool server = options.GetOption("server").as<bool>();
  if (server) {
    int port = options.GetOption("port").as<int>();
    string ip = options.GetOption("ip").as<std::string>();
    return RunServer(ip + ":" + std::to_string(port), plugin, options, ecb);
  }
#endif

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
  else
    mode = emode_invalid;

  if (mode == emode_invalid) {
    errorStream << "Invalid game mode: " << _mode << " aborting!" << std::endl;
    return OPTIONS_ERROR;
  }

  Game game;
  std::string input_file = options.GetOption("input").as<std::string>();
  if (input_file.back() == '/') input_file.pop_back();

  // Working directory hacks
  if (mode != emode_compile)
    game.SetOutputFile(input_file);

  if (input_file.size()) {
    if (!boost::filesystem::exists(input_file)) {
      errorStream << "Input file does not exist: " << input_file << std::endl;
      return OPTIONS_ERROR;
    }

    std::string ext;
    size_t dot = input_file.find_last_of('.');
    if (dot != std::string::npos) ext = tolower(input_file.substr(dot + 1));
    if (ext == "sog") {
      if (!ReadSOG(input_file, &game)) return 1;
      return plugin.BuildGame(game.ConstructGame(), mode, output_file.c_str());
#ifdef CLI_ENABLE_EGM
    } else if (ext == "gm81" || ext == "gmk" || ext == "gm6" || ext == "gmd") {
      buffers::Project* project;
      if (!(project = gmk::LoadGMK(input_file))) return 1;
      return plugin.BuildGame(project->mutable_game(), mode, output_file.c_str());
    } else if (ext == "gmx") {
      boost::filesystem::path p = input_file;
      if (boost::filesystem::is_directory(p)) {
        input_file += "/" + p.filename().stem().string() + ".project.gmx";
      }

      buffers::Project* project;
      if (!(project = gmx::LoadGMX(input_file))) return 1;
      return plugin.BuildGame(project->mutable_game(), mode, output_file.c_str());
    } else if (ext == "yyp") {
      buffers::Project* project;
      if (!(project = yyp::LoadYYP(input_file))) return 1;
      return plugin.BuildGame(project->mutable_game(), mode, output_file.c_str());
#endif
    } else {
      if (ext == "egm") {
        errorStream << "EGM format not yet supported. "
                       "Please use LateralGM for the time being." << std::endl;
      } else if (ext.empty()) {
        errorStream << "Error: Unknown filetype: cannot determine type of file "
                    << '"' << input_file << "\"." << std::endl;
      } else {
        errorStream << "Error: Unknown filetype \"" << ext
                    << "\": cannot read input file \"" << input_file
                    << "\"." << std::endl;
      }
    }
    return 1;
  }

  std::cerr << "Warning: No game file specified. "
               "Building an empty game." << std::endl;
  return plugin.BuildGame(game.ConstructGame(), mode, output_file.c_str());
}
