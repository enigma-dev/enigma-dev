#include "PS_particle.h"
#include "PS_actions.h"

#if ENIGMA_GS_OPENGL1==1
#include "PS_particle_bridge_OpenGL1.h"
#elif ENIGMA_GS_OPENGL3==1
#include "PS_particle_bridge_OpenGL3.h"
#else
#include "PS_particle_bridge_fallback.h"
#endif

