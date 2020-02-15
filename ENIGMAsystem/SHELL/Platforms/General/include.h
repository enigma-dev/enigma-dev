// this is a single include for ALL of the general platform functions
// this file should only be included a single time from each system's
// include.h for systems that intend to support the generic platforms
// and it should be included last in case any of them have already been
// included before

#include "Platforms/General/PFmain.h"
#include "Platforms/General/nonSDL/PFcenter.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFthreads.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/PFexternals.h"
#include "Platforms/General/PFsystem.h"
#include "Platforms/General/PFjoystick.h"
#include "Platforms/General/PFgamepad.h"
