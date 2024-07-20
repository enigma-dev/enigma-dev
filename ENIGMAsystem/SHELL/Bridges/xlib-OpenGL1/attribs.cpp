#if (defined(__APPLE__) && defined(__MACH__))
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <GL/glx.h>

namespace enigma {

int context_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
    GLX_CONTEXT_MINOR_VERSION_ARB, 1,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
    None
};

}
