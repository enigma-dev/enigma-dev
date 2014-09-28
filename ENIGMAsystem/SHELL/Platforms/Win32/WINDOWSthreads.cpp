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

#include "Universal_System/var4.h"
#include "Universal_System/resource_data.h"
#include "../General/PFthreads.h"
#include "WINDOWSmain.h"

std::deque<ethread*> threads;

static void* thread_script_func(void* data) {
  const scrtdata* const md = (scrtdata*)data;
  md->mt->ret = enigma_user::script_execute(md->scr,md->args[0],md->args[1],md->args[2],md->args[3],md->args[4],md->args[5],md->args[6],md->args[7]);
  md->mt->active = false;
  CloseHandle(md->mt->handle);
  return NULL;
}

namespace enigma_user
{

int script_thread(int scr,variant arg0, variant arg1, variant arg2, variant arg3, variant arg4, variant arg5, variant arg6, variant arg7) {
  int thread = thread_create_script(scr,arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7);
  int res = thread_start(thread);
  if (res != 0) {
    thread_delete(thread);
    return res;
  }
  return thread;
}

int thread_create_script(int scr,variant arg0, variant arg1, variant arg2, variant arg3, variant arg4, variant arg5, variant arg6, variant arg7)
{
  ethread* newthread = new ethread();
  variant args[] = {arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7};
  newthread->sd = new scrtdata(scr, args, newthread);
  threads.push_back(newthread);
  return threads.size() - 1;
}

int thread_start(int thread) {
  if (threads[thread]->active) { return -1; }
  
  DWORD dwThreadId;
  threads[thread]->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&thread_script_func, (LPVOID)threads[thread]->sd, 0, &dwThreadId);
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
      MSG msg;
      while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) { 
        TranslateMessage (&msg);
        DispatchMessage (&msg);
      } 
    }
  } else {
    WaitForSingleObject(threads[thread]->handle, INFINITE);
  }
}

void thread_delete(int thread) {
  if (threads[thread]->active) { return; }
  delete threads[thread];
}

bool thread_exists(int thread) {
  return threads[thread] != NULL;
}

bool thread_get_finished(int thread) {
  return !threads[thread]->active;
}

variant thread_get_return(int thread) {
  return threads[thread]->ret;
}

}
