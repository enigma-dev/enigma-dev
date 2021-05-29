#include "Main.hpp"
#include "OptionsParser.hpp"
#include "EnigmaPlugin.hpp"
#include "Server.hpp"

#include "egm.h"
#include "gmk.h"
#include "gmx.h"
#include "yyp.h"
#include "sog.h"

#include "strings_util.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <iostream>
#include <streambuf>
#include <cstdlib>

std::ostream outputStream(nullptr);
std::ostream errorStream(nullptr);

#ifdef _WIN32
/* Tempfix for fs::temp_directory_path() on Michaelsoft Binbows
Broken in MinGW and Visual Studio since std::fs's conception */
#define TMPDIR fs::path(std::getenv("TMP") ? : "C:/").string()
#else
#define TMPDIR fs::temp_directory_path().string()
#endif

int main(int argc, char* argv[])
{
  std::ofstream egmlog(TMPDIR + "/enigma_libegm.log", std::ofstream::out);
  std::ofstream elog(TMPDIR + "/enigma_compiler.log", std::ofstream::out);

  std::string ENIGMA_DEBUG = (std::getenv("ENIGMA_DEBUG") ? std::getenv("ENIGMA_DEBUG") : "");
  if (ENIGMA_DEBUG == "TRUE") {
    outputStream.rdbuf(std::cout.rdbuf());
    errorStream.rdbuf(std::cerr.rdbuf());
  } else {
    outputStream.rdbuf(egmlog.rdbuf());
    errorStream.rdbuf(egmlog.rdbuf());
    std::cout << "LibEGM parsing log at: " << TMPDIR << "/enigma_libegm.log" << std::endl;
    std::cout << "ENIGMA compiler log at: " << TMPDIR << "/enigma_compiler.log" << std::endl;
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

  egm::BindOutputStreams(outputStream, errorStream);

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

  bool server = options.GetOption("server").as<bool>();
  if (server) {
    int port = options.GetOption("port").as<int>();
    string ip = options.GetOption("ip").as<std::string>();
    return RunServer(ip + ":" + std::to_string(port), plugin, options, ecb);
  }

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
  
  std::string input_file = options.GetOption("input").as<std::string>();

  std::unique_ptr<buffers::Project> project;
  
  if (input_file.empty()) {
    project = std::make_unique<buffers::Project>();
    std::cerr << "Warning: No game file specified. "
                "Building an empty game." << std::endl;
    return plugin.BuildGame(project->game(), mode, output_file.c_str());
  }

  // Load event data
  EventData event_data(ParseEventFile((fs::path(options.EnigmaRoot())/"events.ey").u8string()));

  // FIXME: move SOG to libEGM as a format and remove this dup logic
  if (!input_file.empty() && input_file.back() == '/') input_file.pop_back();
  std::string ext;
  size_t dot = input_file.find_last_of('.');
  if (dot != std::string::npos) ext = ToLower(input_file.substr(dot + 1));
  egm::LibEGMInit(&event_data);
  if (!(project = egm::LoadProject(input_file))) return 1;
    return plugin.BuildGame(project->game(), mode, output_file.c_str());
    
  return 1;
}
