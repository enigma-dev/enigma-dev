#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

namespace enigma {

extern HWND hWnd;
extern HINSTANCE hInstance;

}; //namespace enigma
