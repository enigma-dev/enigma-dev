#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include "Bridges/Win32/WINDOWShandle.h"
#include "Platforms/General/PFwindow.h"

namespace enigma_user {

bool window_has_focus() {
  return (enigma::hWnd == GetForegroundWindow());
}

} // namespace enigma_user
