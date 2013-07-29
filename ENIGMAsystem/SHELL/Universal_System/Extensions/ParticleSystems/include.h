#include "PS_particle.h"
#include "PS_actions.h"

#if defined(ENIGMA_GS_OPENGL1) && ENIGMA_GS_OPENGL1
#include "PS_particle_bridge_OpenGL1.h"
#elif defined(ENIGMA_GS_OPENGL3) && ENIGMA_GS_OPENGL3
#include "PS_particle_bridge_OpenGL3.h"
#elif defined(ENIGMA_GS_DIRECT3D9) && ENIGMA_GS_DIRECT3D9
#include "PS_particle_bridge_Direct3D9.h"
#else
#include "PS_particle_bridge_fallback.h"
#endif

