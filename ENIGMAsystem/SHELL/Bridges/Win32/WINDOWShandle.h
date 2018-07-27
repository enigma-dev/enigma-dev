// This function is called by d3d and dsound to ensure the window handle is set
// it is also used in sdl on windows

#include <windows.h>

namespace enigma {

HWND get_window_handle();

}; //namespace enigma
