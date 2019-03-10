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

#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/SDL/Window.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace enigma {

int msaa_fbo = 0;

SDL_GLContext context;

void set_sdl_gl_context_version();

bool initGameWindow() {
  SDL_Init(SDL_INIT_VIDEO);
  set_sdl_gl_context_version();
  SDL_GL_SetSwapInterval(0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  windowHandle = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  return (windowHandle != nullptr);
}

void EnableDrawing(void*) {
  context = SDL_GL_CreateContext(windowHandle);

  GLenum err = glewInit();
  if (GLEW_OK != err)
    show_error(std::string("Failed to initialize glew for OpenGL. ") + glewGetErrorString(err), true);
}

void DisableDrawing(void*) {
  SDL_GL_DeleteContext(context);
}

void ScreenRefresh() {
  SDL_GL_SwapWindow(windowHandle);
}

}

namespace enigma_user {
  void set_synchronization(bool enable) {
    SDL_GL_SetSwapInterval(enable);
  }

  void display_reset(int samples, bool vsync) {
    set_synchronization(vsync);
  }
}
