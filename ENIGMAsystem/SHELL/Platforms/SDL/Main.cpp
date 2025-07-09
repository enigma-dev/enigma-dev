#include "Platforms/General/PFmain.h"
#include <apifilesystem/filesystem.hpp>

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...

namespace enigma {
    void initialize_directory_globals() {
        enigma_user::working_directory = ngs::fs::directory_get_current_working();
        enigma_user::program_directory = ngs::fs::executable_get_directory();
        enigma_user::temp_directory = ngs::fs::directory_get_temporary_path();
    }

    void initialize_imgui_widgets_settings() {
        // File dialogs default window width and height:
        ngs::fs::environment_set_variable("IMGUI_DIALOG_WIDTH", std::to_string(720));
        ngs::fs::environment_set_variable("IMGUI_DIALOG_HEIGHT", std::to_string(382));

        // Widgets grayscale color scheme for imgui dialogs:
        ngs::fs::environment_set_variable("IMGUI_DIALOG_THEME", std::to_string(2));
        ngs::fs::environment_set_variable("IMGUI_TEXT_COLOR_0", std::to_string(1));
        ngs::fs::environment_set_variable("IMGUI_TEXT_COLOR_1", std::to_string(1));
        ngs::fs::environment_set_variable("IMGUI_TEXT_COLOR_2", std::to_string(1));
        ngs::fs::environment_set_variable("IMGUI_HEAD_COLOR_0", std::to_string(0.35));
        ngs::fs::environment_set_variable("IMGUI_HEAD_COLOR_1", std::to_string(0.35));
        ngs::fs::environment_set_variable("IMGUI_HEAD_COLOR_2", std::to_string(0.35));
        ngs::fs::environment_set_variable("IMGUI_AREA_COLOR_0", std::to_string(0.05));
        ngs::fs::environment_set_variable("IMGUI_AREA_COLOR_1", std::to_string(0.05));
        ngs::fs::environment_set_variable("IMGUI_AREA_COLOR_2", std::to_string(0.05));
        ngs::fs::environment_set_variable("IMGUI_BODY_COLOR_0", std::to_string(1));
        ngs::fs::environment_set_variable("IMGUI_BODY_COLOR_1", std::to_string(1));
        ngs::fs::environment_set_variable("IMGUI_BODY_COLOR_2", std::to_string(1));
        ngs::fs::environment_set_variable("IMGUI_POPS_COLOR_0", std::to_string(0.07));
        ngs::fs::environment_set_variable("IMGUI_POPS_COLOR_1", std::to_string(0.07));
        ngs::fs::environment_set_variable("IMGUI_POPS_COLOR_2", std::to_string(0.07));

        #ifdef SDL_VIDEO_DRIVER_X11
        // Prefer Xlib/X11 over Wayland to be compatible with widgets:
        ngs::fs::environment_set_variable("SDL_VIDEODRIVER", "x11");
        #endif
    }
} // namespace enigma

namespace enigma_user {

    std::string filename_absolute(std::string fname) {
        ngs::fs::filename_absolute(fname);
    }

    std::string environment_get_variable(std::string name){
        ngs::fs::environment_get_variable(name);
    }

    std::string environment_unset_variable(std::string name){
        ngs::fs::environment_unset_variable(name);
    }

    bool environment_set_variable(const std::string& name, const std::string& value){
        ngs::fs::environment_set_variable(name, value);
    }
    
    bool set_working_directory(std::string dname) {
        ngs::fs::directory_set_current_working(dname);
    }

} // namespace enigma_user

int main(int argc, char** argv) {
    // Disable high DPI scaling (remain pixel-exact):
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

    // Stop SDL from forcefully disabling compositor for no reason: 
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

    // Widgets/imgui dialogs settings:
    enigma::initialize_imgui_widgets_settings();
    
    return enigma::enigma_main(argc, argv);
}
