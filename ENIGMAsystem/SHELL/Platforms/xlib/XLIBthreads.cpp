/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <deque>
#include <stdio.h>
#include <pthread.h>
#include "../../Universal_System/var4.h"
#include "../../Universal_System/resource_data.h"
#include "XLIBthreads.h"

using namespace std;

struct ethread
{
  pthread_t me;
  bool active;
  variant ret;
  ethread(): me(-1), active(true), ret(0) {};
};

static deque<ethread*> threads;

struct scrtdata {
  int scr;
  variant args[8];
  ethread* mt;
  scrtdata(int s, variant nargs[8], ethread* mythread): scr(s), mt(mythread) { for (int i = 0; i < 8; i++) args[i] = nargs[i]; }
};
static void* thread_script_func(void* data) {
  const scrtdata* const md = (scrtdata*)data;
  md->mt->ret = script_execute(md->scr,md->args[0],md->args[1],md->args[2],md->args[3],md->args[4],md->args[5],md->args[6],md->args[7]);
  md->mt->active = false;
  return NULL;
}

int script_thread(int scr,variant arg0, variant arg1, variant arg2, variant arg3, variant arg4, variant arg5, variant arg6, variant arg7)
{
  ethread* newthread = new ethread();
  scrtdata *sd = new scrtdata(scr,(variant[]){arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7}, newthread);
  if (pthread_create(&newthread->me, NULL, thread_script_func, sd)) {
    delete sd; delete newthread;
    return -1;
  }
  const int ret = threads.size();
  threads.push_back(newthread);
  return ret;
}
bool thread_finished(int thread) {
  return !threads[thread]->active;
}
variant thread_get_return(int thread) {
  return threads[thread]->ret;
}
