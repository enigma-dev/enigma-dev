/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
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

#include "WINDOWSmain.h"

#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFthreads.h"
#include "Platforms/General/PFthreads_impl.h"

using enigma::ethread;
using enigma::threads;

namespace enigma_user {

int thread_start(int thread) {
  if (threads[thread]->active) { return -1; }

  DWORD dwThreadId;
  threads[thread]->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&enigma::thread_script_func, (LPVOID)threads[thread]->sd, 0, &dwThreadId);
  //TODO: May need to check if ret is -1L, and yes it is quite obvious the return value is
  //an unsigned integer, but Microsoft says to for some reason. See their documentation here.
  //http://msdn.microsoft.com/en-us/library/kdzttdcb.aspx
  //NOTE: Same issue is in Universal_Systems/Extensions/Asynchronous/ASYNCdialog.cpp
  if (threads[thread]->handle == NULL) {
    return -2;
  }
  threads[thread]->active = true;
  return 0;
}

void thread_join(int thread) {
  if (GetCurrentThread() == enigma::mainthread) {
    while (WaitForSingleObject(threads[thread]->handle, 10) == WAIT_TIMEOUT) {
      enigma::handleEvents(false);
    }
  } else {
    WaitForSingleObject(threads[thread]->handle, INFINITE);
  }
}

} //namespace enigma_user
