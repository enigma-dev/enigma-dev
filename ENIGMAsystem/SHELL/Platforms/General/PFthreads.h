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

#ifndef ENIGMA_PLATFORM_THREADS_H
#define ENIGMA_PLATFORM_THREADS_H

#include "Universal_System/var4.h"

namespace enigma_user {
  int script_thread(int scr, evariant arg0 = 0, evariant arg1 = 0, evariant arg2 = 0, evariant arg3 = 0, evariant arg4 = 0, evariant arg5 = 0, evariant arg6 = 0, evariant arg7 = 0);
  int thread_create_script(int scr, evariant arg0 = 0, evariant arg1 = 0, evariant arg2 = 0, evariant arg3 = 0, evariant arg4 = 0, evariant arg5 = 0, evariant arg6 = 0, evariant arg7 = 0);
  int thread_start(int thread);
  void thread_join(int thread);
  void thread_delete(int thread);
  bool thread_exists(int thread);
  bool thread_get_finished(int thread);
  evariant thread_get_return(int thread);
} //namespace enigma_user

#endif //ENIGMA_PLATFORM_THREADS_H
