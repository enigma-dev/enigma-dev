#include "Main.hpp"
#include "OptionsParser.hpp"
#include "EnigmaPlugin.hpp"
#include "Game.hpp"

#ifdef CLI_ENABLE_SERVER
#include "Server.hpp"
#endif

#include "SOG.hpp"

#ifdef CLI_ENABLE_EGM
#include "egm.h"
#include "gmk.h"
#include "gmx.h"
#include "yyp.h"
#endif

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <iostream>
#include <streambuf>
#include <cstdlib>

std::ostream outputStream(nullptr);
std::ostream errorStream(nullptr);

static std::string tolower(const std::string &str) {
  std::string res = str;
  for (size_t i = 0; i < res.length(); ++i) {
    if (res[i] >= 'A' && res[i] <= 'Z') res[i] += 'a' - 'A';
  }
  return res;
}

int main(int argc, char* argv[])
{
  std::ofstream egmlog(fs::temp_directory_path().string() + "/enigma_libegm.log", std::ofstream::out);
  std::ofstream elog(fs::temp_directory_path().string() + "/enigma_compiler.log", std::ofstream::out);

  std::string ENIGMA_DEBUG = (std::getenv("ENIGMA_DEBUG") ? std::getenv("ENIGMA_DEBUG") : "");
  if (ENIGMA_DEBUG == "TRUE") {
    outputStream.rdbuf(std::cout.rdbuf());
    errorStream.rdbuf(std::cerr.rdbuf());
  } else {
    outputStream.rdbuf(egmlog.rdbuf());
    errorStream.rdbuf(egmlog.rdbuf());
    std::cout << "LibEGM parsing log at: " << fs::temp_directory_path().string() << "/enigma_libegm.log" << std::endl;
    std::cout << "ENIGMA compiler log at: " << fs::temp_directory_path().string() << "/enigma_compiler.log" << std::endl;
  }
  
  
  OptionsParser options;
  options.ReadArgs(argc, argv);
  int result = options.HandleArgs();

  if (result == OPTIONS_ERROR || result == OPTIONS_HELP)
    return result;

  EnigmaPlugin plugin;
  plugin.Load();
  CallBack ecb;
  plugin.Init(&ecb, options.EnigmaRoot());
  
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

  std::streambuf* cout_rdbuf = std::cout.rdbuf();
  std::streambuf* cerr_rdbuf = std::cerr.rdbuf();
    
  if (ENIGMA_DEBUG != "TRUE") {
    // Hijack cout/cerr from plugin to hide jdi startup crap that scares people
    std::cout.rdbuf(elog.rdbuf());
    std::cerr.rdbuf(elog.rdbuf());
  }
  
  plugin.SetDefinitions(options.APIyaml().c_str());
  std::string output_file;

  if (ENIGMA_DEBUG != "TRUE") {
    //Restore cout/cerr
    std::cout.rdbuf(cout_rdbuf);
    std::cerr.rdbuf(cerr_rdbuf);
  }

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
    std::cerr << "Invalid game mode: " << _mode << " aborting!" << std::endl;
    return OPTIONS_ERROR;
  }

  Game game;
  std::string input_file = options.GetOption("input").as<std::string>();
  if (!input_file.empty() && input_file.back() == '/') input_file.pop_back();

  // Working directory hacks
  if (mode != emode_compile)
    game.SetOutputFile(input_file);

  if (input_file.size()) {
    if (!fs::exists(input_file)) {
      std::cerr << "Input file does not exist: " << input_file << std::endl;
      return OPTIONS_ERROR;
    }

    EventData event_data(ParseEventFile((fs::path(options.EnigmaRoot())/"events.ey").u8string()));

    // FIXME: Remove all occurrences of wrap_unique in the following code.
    // There is no reason for these projects to be bare pointers.
    std::string ext;
    size_t dot = input_file.find_last_of('.');
    std::unique_ptr<buffers::Project> project;
    if (dot != std::string::npos) ext = tolower(input_file.substr(dot + 1));
    if (ext == "sog") {
      if (!ReadSOG(input_file, &game, &event_data)) return 1;
      return plugin.BuildGame(game.ConstructGame(), mode, output_file.c_str());
#ifdef CLI_ENABLE_EGM
    } else if (ext == "gm81" || ext == "gmk" || ext == "gm6" || ext == "gmd") {
      if (!(project = gmk::LoadGMK(input_file, &event_data))) return 1;
      return plugin.BuildGame(project->game(), mode, output_file.c_str());
    } else if (ext == "gmx") {
      fs::path p = input_file;
      if (fs::is_directory(p)) {
        input_file += "/" + p.filename().stem().string() + ".project.gmx";
      }

      if (!(project = gmx::LoadGMX(input_file, &event_data))) return 1;
      return plugin.BuildGame(project->game(), mode, output_file.c_str());
    } else if (ext == "yyp") {
      if (!(project = yyp::LoadYYP(input_file, &event_data))) return 1;
      return plugin.BuildGame(project->game(), mode, output_file.c_str());
    } else if (ext == "egm") {
      fs::path p = input_file;
      if (fs::is_directory(p)) {
        input_file += "/" + p.filename().stem().string() + ".egm";
      }
      egm::EGM egm(&event_data);
      if (!(project = egm.LoadEGM(input_file))) return 1;
      return plugin.BuildGame(project->game(), mode, output_file.c_str());
    } else if (ext.empty()) {
      std::cerr << "Error: Unknown filetype: cannot determine type of file "
                  << '"' << input_file << "\"." << std::endl;
#endif
    } else {
      std::cerr << "Error: Unknown filetype \"" << ext
                  << "\": cannot read input file \"" << input_file
                  << "\"." << std::endl;
    }
    return 1;
  }

  std::cerr << "Warning: No game file specified. "
               "Building an empty game." << std::endl;
  return plugin.BuildGame(game.ConstructGame(), mode, output_file.c_str());
}
