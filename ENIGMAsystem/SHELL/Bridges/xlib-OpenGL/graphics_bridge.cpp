/** Copyright (C) 2013 forthevin
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

#include "OpenGLHeaders.h"
#include "Graphics_Systems/OpenGL-Common/version.h"
#include "Graphics_Systems/OpenGL-Common/shader.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Bridges/OpenGL/GLload.h"

#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/xlib/XLIBwindow.h"

#include <X11/Xlib.h>
#include <GL/glxew.h>

#include <cstring>

// NOTE: Changes/fixes that applies to this likely also applies to the OpenGL3 version.

namespace enigma {
  extern int context_attribs[];
  GLXContext glxc;
  GLXFBConfig* fbc;

  extern void (*WindowResizedCallback)();
  void WindowResized() {
    glViewport(0,0,enigma_user::window_get_width(),enigma_user::window_get_height());
    glScissor(0,0,enigma_user::window_get_width(),enigma_user::window_get_height());
    enigma_user::draw_clear(enigma_user::window_get_color());
  }

  GLXFBConfig* CreateFBConfig() {
    // Prepare openGL
    GLint att[] = { GLX_DOUBLEBUFFER, True, GLX_DEPTH_SIZE, 24, None };
    int config_count;
    fbc = glXChooseFBConfig(enigma::x11::disp, 0, att, &config_count);
    if (!fbc || config_count < 1)
      DEBUG_MESSAGE("Failed to Obtain GL Frambuffer config", MESSAGE_TYPE::M_FATAL_ERROR);
    return fbc;
  }

  void EnableDrawing(void* handle) {
    WindowResizedCallback = &WindowResized;

    //give us a GL context
    glxc = glXCreateContextAttribsARB(enigma::x11::disp, fbc[0], 0, True, context_attribs);
    if (!glxc)
      DEBUG_MESSAGE("Failed to Create Graphics Context", MESSAGE_TYPE::M_FATAL_ERROR);

    //apply context
    glXMakeCurrent(enigma::x11::disp,enigma::x11::win,glxc); //flushes
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    gl_load_exts();
  }

  void DisableDrawing(void* handle) {
   cleanup_shaders(); // delete shaders before context
   glXDestroyContext(enigma::x11::disp,glxc);
  }

  namespace swaphandling {
    static bool has_checked_extensions = false;
    static bool ext_swapcontrol_supported;
    static bool mesa_swapcontrol_supported;

    static void investigate_swapcontrol_support() {
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

  static bool is_ext_swapcontrol_supported() {
    swaphandling::investigate_swapcontrol_support();
    return swaphandling::ext_swapcontrol_supported;
  }
  static bool is_mesa_swapcontrol_supported() {
    swaphandling::investigate_swapcontrol_support();
    return swaphandling::mesa_swapcontrol_supported;
  }

  void ScreenRefresh() {
    glXSwapBuffers(enigma::x11::disp, enigma::x11::win);
  }
}

namespace enigma_user {
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
}
