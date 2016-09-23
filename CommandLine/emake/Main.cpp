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
        plugin.SetDefinitions(options.APIyaml());

        Game game;

        GameMode mode;

        if (options.GetOption("mode") == "Compile")
            mode = emode_compile;
        else if (options.GetOption("mode") == "Run")
            mode = emode_run;
        else if (options.GetOption("mode") == "Debug")
            mode = emode_debug;
        else if (options.GetOption("mode") == "Design")
            mode = emode_design;
        else if (options.GetOption("mode") == "Rebuild")
            mode = emode_rebuild;


        return plugin.BuildGame(game.ConstructGame(), mode, options.GetOption("output").c_str());
    }

    return result;
}
