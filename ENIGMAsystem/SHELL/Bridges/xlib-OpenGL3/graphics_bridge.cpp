/** Copyright (C) 2013 forthevin
***               2014 Harijs Grinbergs
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
//#include <GL/glx.h>
#include <X11/Xlib.h>
#include "../General/glxew.h"
#include "Platforms/xlib/XLIBmain.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"

#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <string>
#include <iostream>
#include <cstring>
#include <stdio.h>

// NOTE: Changes/fixes that applies to this likely also applies to the OpenGL1 version.

namespace enigma {
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
  GLXContext glxc;
  GLXFBConfig *fbc;
  XVisualInfo *vi;

  XVisualInfo* CreateVisualInfo() {
    // Prepare openGL
    // Get a matching FB config
    static int visual_attribs[] =
    {
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,
      //GLX_SAMPLE_BUFFERS  , 1,
      //GLX_SAMPLES         , 4,
      None
    };

    int fbcount;
    fbc = glXChooseFBConfig(enigma::x11::disp, DefaultScreen(enigma::x11::disp), visual_attribs, &fbcount);

    if(!fbc){
        printf("Failed to Obtain GL Config\n");
        return NULL;
    }
    vi = glXGetVisualFromFBConfig( enigma::x11::disp, fbc[0] );
    return vi;
  }

  void EnableDrawing() {

    // -- Initialise GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      return;
    }


    static int attribs[] =
    {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
      GLX_CONTEXT_MINOR_VERSION_ARB, 3,
      //GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      #ifdef DEBUG_MODE
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
      #else
        GLX_CONTEXT_FLAGS_ARB, 0,
      #endif
      0
    };

    glxc = glXCreateContextAttribsARB( enigma::x11::disp, fbc[0], NULL, True, attribs );

    // Sync to ensure any errors generated are processed.
    XSync( enigma::x11::disp, False );
    if (!glxc)
    {
      // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
      attribs[1] = 1;
      // GLX_CONTEXT_MINOR_VERSION_ARB = 0
      attribs[3] = 0;

      printf( "Failed to create GL 3.0 context. Using fallback GLX context\n" );
      glxc = glXCreateContextAttribsARB( enigma::x11::disp, fbc[0], NULL,
                                        True, attribs );
      if (!glxc) {
        printf("Failed to create fallback context\n");
        return;
      }
    }

    #ifdef DEBUG_MODE
    glDebugMessageCallbackARB((GLDEBUGPROCARB)&DebugCallbackARB, 0);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

    GLuint ids[] = { 131185 };
    glDebugMessageControlARB(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 1, ids, GL_FALSE); //Disable notification about rendering HINTS like so:
    //OpenGL: Buffer detailed info: Buffer object 1 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations. [source=API type=OTHER severity=UNDEFINED (33387) id=131185]
    #endif

    //apply context
    glXMakeCurrent(enigma::x11::disp,enigma::x11::win,glxc); //flushes
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
  }

  void DisableDrawing() {
   glXDestroyContext(enigma::x11::disp,glxc);
      /*
    for(char q=1;q;ENIGMA_events())
        while(XQLength(disp))
            if(handleEvents()>0) q=0;
    glxc = glXGetCurrentContext();
    glXDestroyContext(disp,glxc);
    XCloseDisplay(disp);
    return 0;*/
  }

  void WindowResized() {
    glViewport(0,0,enigma_user::window_get_width(),enigma_user::window_get_height());
    glScissor(0,0,enigma_user::window_get_width(),enigma_user::window_get_height());
    enigma_user::draw_clear(enigma_user::window_get_color());
  }

  namespace swaphandling {
    bool has_checked_extensions = false;
    bool ext_swapcontrol_supported;
    bool mesa_swapcontrol_supported;

    void investigate_swapcontrol_support() {

      if (has_checked_extensions) return; // Already calculated, no need to calculate it more.

      // TODO: The second argument to glXQueryExtensionsString is screen number,
      // and it is unknown if the value 0 is generally correct for calling that function.
      // For more information, see the following pages:
      // http://pic.dhe.ibm.com/infocenter/aix/v6r1/index.jsp?topic=%2Fcom.ibm.aix.opengl%2Fdoc%2Fopenglrf%2FglXQueryExtensionsString.htm
      const char *glx_extensions = glXQueryExtensionsString(enigma::x11::disp, 0);

      ext_swapcontrol_supported = strstr(glx_extensions, "GLX_EXT_swap_control");
      mesa_swapcontrol_supported = strstr(glx_extensions, "GLX_MESA_swap_control");

      has_checked_extensions = true;
    }
  }

  bool is_ext_swapcontrol_supported() {
    swaphandling::investigate_swapcontrol_support();
    return swaphandling::ext_swapcontrol_supported;
  }
  bool is_mesa_swapcontrol_supported() {
    swaphandling::investigate_swapcontrol_support();
    return swaphandling::mesa_swapcontrol_supported;
  }
}

#include <Platforms/xlib/XLIBwindow.h> // window_set_caption
#include <Universal_System/roomsystem.h> // room_caption, update_mouse_variables

namespace enigma_user {
// Don't know where to query this on XLIB, just defaulting it to 2,4,and 8 samples all supported, Windows puts it in EnableDrawing
int display_aa = 14;

void set_synchronization(bool enable) {
	// General notes:
	// Setting swapping on and off is platform-dependent and requires platform-specific extensions.
	// Platform-specific extensions are even more bothersome than regular extensions.
	// What functions and features to use depends on which version of OpenGL is used.
	// For more information, see the following pages:
	// http://www.opengl.org/wiki/Load_OpenGL_Functions
	// http://www.opengl.org/wiki/OpenGL_Loading_Library
	// http://www.opengl.org/wiki/Swap_Interval
	// http://en.wikipedia.org/wiki/GLX
	// Also note that OpenGL version >= 3.0 does not use glGetString for getting extensions.

	if (enigma::x11::disp != 0) {
	  GLXDrawable drawable = glXGetCurrentDrawable();

	  int interval = enable ? 1 : 0;

	  if (enigma::is_ext_swapcontrol_supported()) {
		glXSwapIntervalEXT(enigma::x11::disp, drawable, interval);
	  }
	  else if (enigma::is_mesa_swapcontrol_supported()) {
		glXSwapIntervalMESA(interval);
	  }
	  // NOTE: GLX_SGI_swap_control, which is not used here, does not seem
	  // to support disabling of synchronization, since its argument may not
	  // be zero or less, so therefore it is not used here.
	  // See http://www.opengl.org/registry/specs/SGI/swap_control.txt for more information.
	}
}

void display_reset(int samples, bool vsync) {
	set_synchronization(vsync);
	//TODO: Copy over from the Win32 bridge
}

void screen_refresh() {
	glXSwapBuffers(enigma::x11::disp, enigma::x11::win);
	enigma::update_mouse_variables();
	window_set_caption(room_caption);
}

}

