#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFsystem.h"

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...

#if defined(_WIN32)
#define os_type enigma_user::os_windows
#elif (defined(__APPLE__) && defined(__MACH__))
#define os_type enigma_user::os_macosx
#elif defined(__linux__)
#define os_type enigma_user::os_linux
#elif defined(__FreeBSD__)
#define os_type enigma_user::os_freebsd
#elif defined(__DragonFly__)
#define os_type enigma_user::os_dragonfly
#elif defined(__NetBSD__)
#define os_type enigma_user::os_netbsd
#elif defined(__OpenBSD__)
#define os_type enigma_user::os_openbsd
#elif defined(__sun)
#define os_type enigma_user::os_sunos
#else
#define os_type enigma_user::os_unknown
#endif

int main(int argc, char** argv) {
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
  return enigma::enigma_main(argc, argv);
}
