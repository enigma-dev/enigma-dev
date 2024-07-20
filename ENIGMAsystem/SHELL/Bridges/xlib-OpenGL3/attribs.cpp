#if (defined(__APPLE__) && defined(__MACH__))
#include <OpenGL/gl.h>
#include <OpenGL/glx.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif

namespace enigma {

int context_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 3,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    #ifdef DEBUG_MODE
    GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
    #endif
    None
};

}
