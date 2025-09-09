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
        #ifdef SDL_VIDEO_DRIVER_X11
        // Prefer Xlib/X11 over Wayland to be compatible with widgets:
        ngs::fs::environment_set_variable("SDL_VIDEODRIVER", "x11");
        #endif
    }
} // namespace enigma

int main(int argc, char** argv) {
    // Disable high DPI scaling (remain pixel-exact):
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

    // Stop SDL from forcefully disabling compositor for no reason: 
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

    // Widgets/imgui dialogs settings:
    enigma::initialize_imgui_widgets_settings();
    
    return enigma::enigma_main(argc, argv);
}
