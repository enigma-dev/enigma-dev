// This function is called by d3d and dsound to ensure the window handle is set 
// it also used in sdl on windows

#include <windows.h>

namespace enigma {
//extern HWND hWnd;
HWND get_window_handle();

}; //namespace enigma
