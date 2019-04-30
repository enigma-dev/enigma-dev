/** Copyright (C) 2018 Greg Williamson
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

#include "Bridges/OpenGL/GLload.h"
#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/SDL/Window.h"

#include <SDL2/SDL.h>

namespace enigma {

extern unsigned sdl_window_flags;

SDL_GLContext context;

void init_sdl_window_bridge_attributes() {
  #ifdef DEBUG_MODE
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
  #endif
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    graphics_opengl_core?SDL_GL_CONTEXT_PROFILE_CORE:SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  SDL_GL_SetSwapInterval(0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  sdl_window_flags |= SDL_WINDOW_OPENGL;
}

void EnableDrawing(void*) {
  context = SDL_GL_CreateContext(windowHandle);

  gl_load_exts();
}

void DisableDrawing(void*) {
  SDL_GL_DeleteContext(context);
}

void ScreenRefresh() {
  SDL_GL_SwapWindow(windowHandle);
}

} // namespace enigma

namespace enigma_user {

void set_synchronization(bool enable) {
  SDL_GL_SetSwapInterval(enable);
}

} // namespace enigma_user
