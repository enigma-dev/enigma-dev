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

//#include "../../../../../CompilerSource/OS_Switchboard.h" //TODO: Possibly not needed?

#if CURRENT_PLATFORM_ID == OS_WINDOWS
#include <process.h>
#else
#include <pthread.h> // use POSIX threads
#endif

#include <deque>
#include <stdio.h>
using namespace std;

#include "Universal_System/var4.h"

struct ethread
{
#if CURRENT_PLATFORM_ID != OS_WINDOWS
  pthread_t me;
#endif
  bool active;
  variant ret;
  ethread(): active(true), ret(0) {};
};

static deque<ethread*> threads;

namespace enigma_user {
int script_thread(int scr, variant arg0 = 0, variant arg1 = 0, variant arg2 = 0, variant arg3 = 0, variant arg4 = 0, variant arg5 = 0, variant arg6 = 0, variant arg7 = 0);
bool thread_get_finished(int thread);
variant thread_get_return(int thread);
}

#endif //ENIGMA_PLATFORM_THREADS_H
