#ifdef __APPLE__
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#endif

#if defined(_Win32)
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#endif

#include <GL/glew.h>
