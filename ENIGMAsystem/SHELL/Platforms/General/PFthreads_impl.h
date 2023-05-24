/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
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

#ifndef ENIGMA_PLATFORM_THREADS_IMPL_H
#define ENIGMA_PLATFORM_THREADS_IMPL_H

#include "Universal_System/var4.h"

#ifdef ENIGMA_PLATFORM_SDL
  #include <SDL.h>
  using pltfrm_thread_t = SDL_Thread*;
#elif ENIGMA_PLATFORM_WINDOWS
  #define byte __windows_byte_workaround
#include <windows.h>
#undef byte

  using pltfrm_thread_t = HANDLE;
#else
  #include <pthread.h> // use POSIX threads
  using pltfrm_thread_t = pthread_t;
#endif

#include <deque>

namespace enigma {

struct ethread;

struct scrtdata {
  int scr;
  variant args[8];
  ethread* mt;
  scrtdata(int s, variant nargs[8], ethread* mythread): scr(s), mt(mythread) { for (int i = 0; i < 8; i++) args[i] = nargs[i]; }
};

struct ethread {
  pltfrm_thread_t handle;
  scrtdata *sd;
  bool active;
  variant ret;
  ethread(): handle(0), sd(NULL), active(false), ret(0) {};
  ~ethread() {
    if (sd != NULL) {
      delete sd;
    }
  }
};

extern std::deque<ethread*> threads;

void* thread_script_func(void* data);

} // namespace enigma

#endif //ENIGMA_PLATFORM_THREADS_IMPL_H
