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

#include "Platforms/General/PFthreads.h"
#include "Platforms/General/PFthreads_impl.h"

#include <functional>

using enigma::ethread;
using enigma::threads;

static int _thread_script_func(void* data) {
  enigma::thread_script_func(data);
  return 0;
}

namespace enigma_user {

int thread_start(int thread) {
  if (threads[thread]->active) return -1;
  threads[thread]->handle = SDL_CreateThread(_thread_script_func, NULL, (void *)threads[thread]->sd);

  if (threads[thread]->handle == NULL)
    return -2;

  threads[thread]->active = true;
  return 0;
}

void thread_join(int thread) {
  SDL_WaitThread(threads[thread]->handle, NULL);
}

} //namespace enigma_user
