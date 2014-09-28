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
#include "PFthreads.h"

std::deque<ethread*> threads;

static void* thread_script_func(void* data) {
  const scrtdata* const md = (scrtdata*)data;
  md->mt->ret = enigma_user::script_execute(md->scr,md->args[0],md->args[1],md->args[2],md->args[3],md->args[4],md->args[5],md->args[6],md->args[7]);
  md->mt->active = false;
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
  if (pthread_create(&threads[thread]->handle, NULL, thread_script_func, threads[thread]->sd)) {
    return -2;
  }
  threads[thread]->active = true;
  return 0;
}

void thread_join(int thread) {
  pthread_join(threads[thread]->handle, NULL);
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
