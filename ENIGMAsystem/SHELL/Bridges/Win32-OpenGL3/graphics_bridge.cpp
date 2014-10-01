/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <string>
#include <windows.h>
using namespace std;

#include "../General/wglew.h"
#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"

namespace enigma
{

	#ifdef DEBUG_MODE
	#include "Widget_Systems/widgets_mandatory.h"
	//Based on code from Cort Stratton (http://www.altdev.co/2011/06/23/improving-opengl-error-messages/)
	void FormatDebugOutputARB(char outStr[], size_t outStrSize, GLenum source, GLenum type, GLuint id, GLenum severity, const char *msg) {
		char sourceStr[32]; const char *sourceFmt = "UNDEFINED(0x%04X)";
		switch(source) {
			case GL_DEBUG_SOURCE_API_ARB: sourceFmt = "API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: sourceFmt = "WINDOW_SYSTEM"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: sourceFmt = "SHADER_COMPILER"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: sourceFmt = "THIRD_PARTY"; break;
			case GL_DEBUG_SOURCE_APPLICATION_ARB: sourceFmt = "APPLICATION"; break;
			case GL_DEBUG_SOURCE_OTHER_ARB: sourceFmt = "OTHER"; break;
		}
		snprintf(sourceStr, 32, sourceFmt, source);
		char typeStr[32];
		const char *typeFmt = "UNDEFINED(0x%04X)";
		switch(type) {
			case GL_DEBUG_TYPE_ERROR_ARB: typeFmt = "ERROR"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: typeFmt = "DEPRECATED_BEHAVIOR"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: typeFmt = "UNDEFINED_BEHAVIOR"; break;
			case GL_DEBUG_TYPE_PORTABILITY_ARB: typeFmt = "PORTABILITY"; break;
			case GL_DEBUG_TYPE_PERFORMANCE_ARB: typeFmt = "PERFORMANCE"; break;
			case GL_DEBUG_TYPE_OTHER_ARB: typeFmt = "OTHER"; break;
		}
		snprintf(typeStr, 32, typeFmt, type);
		char severityStr[32];
		const char *severityFmt = "UNDEFINED(%i)";
		switch(severity) {
			case GL_DEBUG_SEVERITY_HIGH_ARB: severityFmt = "HIGH"; break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB: severityFmt = "MEDIUM"; break;
			case GL_DEBUG_SEVERITY_LOW_ARB: severityFmt = "LOW"; break;
		}
		snprintf(severityStr, 32, severityFmt, severity);
		snprintf(outStr, outStrSize, "OpenGL: %s [source=%s type=%s severity=%s id=%d]", msg, sourceStr, typeStr, severityStr, id);
	}

	void DebugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid* userParam) {
		(void)length;
		char finalMessage[256];
		FormatDebugOutputARB(finalMessage, 256, source, type, id, severity, message);
		printf("%s\n", finalMessage);
    show_error(toString(finalMessage), false);
	}
	#endif

	GLuint msaa_fbo = 0;

  void EnableDrawing (HGLRC *hRC)
  {
    /**
     * Edited by Cool Breeze on 16th October 2013
     * + Updated the Pixel Format to support 24-bitdepth buffers
     * + Correctly create a GL 3.x compliant context
     */

    HGLRC LegacyRC;
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    enigma::window_hDC = GetDC (hWnd);
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (enigma::window_hDC, &pfd);

    if (iFormat==0) { show_error("Failed to set the format of the OpenGL graphics device.",1); }

    SetPixelFormat ( enigma::window_hDC, iFormat, &pfd );
    LegacyRC = wglCreateContext( enigma::window_hDC );
    wglMakeCurrent( enigma::window_hDC, LegacyRC );

    // -- Initialise GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      return;
    }

    // -- Define an array of Context Attributes
    int attribs[] =
    {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
      WGL_CONTEXT_MINOR_VERSION_ARB, 3,
      //WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      #ifdef DEBUG_MODE
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
      #else
        WGL_CONTEXT_FLAGS_ARB, 0,
      #endif
      0
    };

    if ( wglewIsSupported("WGL_ARB_create_context") )
    {
      *hRC = wglCreateContextAttribsARB( enigma::window_hDC,0, attribs );
      wglMakeCurrent( NULL,NULL );
      wglDeleteContext( LegacyRC );
      wglMakeCurrent(enigma::window_hDC, *hRC );
    }
    else // Unable to get a 3.3 Core Context, use the Legacy 1.x context
    {
      *hRC = LegacyRC;
    }

    #ifdef DEBUG_MODE
    glDebugMessageCallbackARB((GLDEBUGPROCARB)&DebugCallbackARB, 0);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    #endif

    //TODO: This never reports higher than 8, but display_aa should be 14 if 2,4,and 8 are supported and 8 only when only 8 is supported
    glGetIntegerv(GL_MAX_SAMPLES_EXT, &enigma_user::display_aa);

    GLuint ids[] = { 131185 };
    glDebugMessageControlARB(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 1, ids, GL_FALSE); //Disable notification about rendering HINTS like so:
    //OpenGL: Buffer detailed info: Buffer object 1 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations. [source=API type=OTHER severity=UNDEFINED (33387) id=131185]
  }


  void WindowResized() {
    // clear the window color, viewport does not need set because backbuffer was just recreated
    enigma_user::draw_clear(enigma_user::window_get_color());
  }

  void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC)
  {
      wglMakeCurrent (NULL, NULL);
      wglDeleteContext (hRC);
      ReleaseDC (hWnd, hDC);
  }
}

// NOTE: Changes/fixes that applies to this likely also applies to the OpenGL1 version.

namespace enigma {
  namespace swaphandling {
    bool has_checked_extensions = false;
    bool ext_swapcontrol_supported;

    void investigate_swapcontrol_support() {

      if (has_checked_extensions) return; // Already calculated, no need to calculate it more.

      const char *wgl_extensions = wglGetExtensionsStringARB(window_hDC);

      ext_swapcontrol_supported = strstr(wgl_extensions, "WGL_EXT_swap_control");

      has_checked_extensions = true;
    }
  }

  bool is_ext_swapcontrol_supported() {
    swaphandling::investigate_swapcontrol_support();
    return swaphandling::ext_swapcontrol_supported;
  }
}

#include "Universal_System/roomsystem.h"

namespace enigma_user {
	int display_aa = 0;

	void display_reset(int samples, bool vsync) {
		int interval = vsync ? 1 : 0;

		if (enigma::is_ext_swapcontrol_supported()) {
		  wglSwapIntervalEXT(interval);
		}

		GLint fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);

		GLuint ColorBufferID, DepthBufferID;

		// Cleanup the multi-sampler fbo if turning off multi-sampling
		if (samples == 0) {
			if (enigma::msaa_fbo != 0) {
				glDeleteFramebuffers(1, &enigma::msaa_fbo);
				enigma::msaa_fbo = 0;
			}
			return;
		}

		//TODO: Change the code below to fix this to size properly to views
		// If we don't already have a multi-sample fbo then create one
		if (enigma::msaa_fbo == 0) {
			glGenFramebuffers(1, &enigma::msaa_fbo);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, enigma::msaa_fbo);
		// Now make a multi-sample color buffer
		glGenRenderbuffers(1, &ColorBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, ColorBufferID);
		glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, samples, GL_RGBA8, window_get_region_width(), window_get_region_height());
		// We also need a depth buffer
		glGenRenderbuffersEXT(1, &DepthBufferID);
		glBindRenderbufferEXT(GL_RENDERBUFFER, DepthBufferID);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, window_get_region_width(), window_get_region_height());
		// Attach the render buffers to the multi-sampler fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorBufferID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBufferID);
	}


  void screen_refresh() {
    window_set_caption(room_caption);
    enigma::update_mouse_variables();
    SwapBuffers(enigma::window_hDC);
  }

  void set_synchronization(bool enable) {

    // General notes:
    // Setting swapping on and off is platform-dependent and requires platform-specific extensions.
    // Platform-specific extensions are even more bothersome than regular extensions.
    // What functions and features to use depends on which version of OpenGL is used.
    // For more information, see the following pages:
    // http://www.opengl.org/wiki/Load_OpenGL_Functions
    // http://www.opengl.org/wiki/OpenGL_Loading_Library
    // http://www.opengl.org/wiki/Swap_Interval
    // http://en.wikipedia.org/wiki/WGL_%28software%29
    // Also note that OpenGL version >= 3.0 does not use glGetString for getting extensions.

    int interval = enable ? 1 : 0;

    if (enigma::is_ext_swapcontrol_supported()) {
      wglSwapIntervalEXT(interval);
    }
  }
}
