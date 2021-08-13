#include "Platforms/General/PFmain.h"

#include "Universal_System/mathnc.h" // enigma_user::clamp
#include "Widget_Systems/widgets_mandatory.h"

#include <sys/types.h>     //getpid
#include <unistd.h>        //usleep

namespace enigma {

void Sleep(int ms) {
  if (ms >= 1000) enigma_user::sleep(ms / 1000);
  if (ms > 0) usleep(ms % 1000 * 1000);
}

void windowsystem_write_exename(char* x) {
  unsigned irx = 0;
  if (enigma::parameterc)
    for (irx = 0; enigma::parameters[0][irx] != 0; irx++) x[irx] = enigma::parameters[0][irx];
  x[irx] = 0;
}

}  //namespace enigma
