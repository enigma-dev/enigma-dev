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

#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GScolors.h"

#include "Platforms/General/PFwindow.h"
#include "Platforms/SDL/Window.h"

#include "Universal_System/roomsystem.h" // room_caption, update_mouse_variables

#include <SDL2/SDL.h>

#include <iostream>
#include <cstring>
#include <stdio.h>

namespace enigma {

int msaa_fbo = 0;

SDL_GLContext context;
SDL_Renderer *renderer;

bool initGameWindow() {
  SDL_Init(SDL_INIT_VIDEO);
  windowHandle = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  return (windowHandle != nullptr);
}

void EnableDrawing(void*) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MINOR_VERSION);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    context = SDL_GL_CreateContext(windowHandle);
    renderer = SDL_CreateRenderer(windowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
}

void DisableDrawing(void*) {
  SDL_DestroyRenderer(renderer);
}

}

namespace enigma_user {
  // Don't know where to query this on XLIB, just defaulting it to 2,4,and 8 samples all supported, Windows puts it in EnableDrawing
  int display_aa = 14;

  void set_synchronization(bool enable) {
    SDL_GL_SetSwapInterval(enable);
  }
    
  void display_reset(int samples, bool vsync) {
    set_synchronization(vsync);
  }
    
  void screen_refresh() {
    SDL_GL_SwapWindow(enigma::windowHandle);
    window_set_caption(room_caption);
    enigma::update_mouse_variables();
  }

}

