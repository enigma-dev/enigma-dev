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
//#include <GL/glx.h>
#include <X11/Xlib.h>
#include "../General/glxew.h"
#include "Platforms/xlib/XLIBmain.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"

#include <iostream>
#include <cstring>
#include <stdio.h>

// NOTE: Changes/fixes that applies to this likely also applies to the OpenGL3 version.

namespace enigma {
  GLuint msaa_fbo = 0;
  GLXContext glxc;
  XVisualInfo *vi;
  
  extern void (*WindowResizedCallback)();
  void WindowResized() {
    glViewport(0,0,enigma_user::window_get_width(),enigma_user::window_get_height());
    glScissor(0,0,enigma_user::window_get_width(),enigma_user::window_get_height());
    enigma_user::draw_clear(enigma_user::window_get_color());
  }
  
  XVisualInfo* CreateVisualInfo() {
    // Prepare openGL
    GLint att[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, None };
    vi = glXChooseVisual(enigma::x11::disp,0,att);
    if(!vi){
        printf("Failed to Obtain GL Visual Info\n");
        return NULL;
    }
    return vi;
  }

  void EnableDrawing() {
    WindowResizedCallback = &WindowResized;
    
    //give us a GL context
    glxc = glXCreateContext(enigma::x11::disp, vi, NULL, True);
    if (!glxc){
        printf("Failed to Create Graphics Context\n");
        return;
    }
    
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

