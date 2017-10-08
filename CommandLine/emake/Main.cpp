#include "OptionsParser.hpp"
#include "EnigmaPlugin.hpp"
#include "Game.hpp"

int main(int argc, char* argv[])
{
  OptionsParser options;
  options.ReadArgs(argc, argv);
  int result = options.HandleArgs();

  if (result != OPTIONS_ERROR && result != OPTIONS_HELP)
  {
    EnigmaPlugin plugin;
    plugin.Init();
    plugin.SetDefinitions(options.APIyaml().c_str());

    Game game;

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

    return plugin.BuildGame(game.ConstructGame(), mode, options.GetOption("output").as<std::string>().c_str());
  }

  return result;
}
