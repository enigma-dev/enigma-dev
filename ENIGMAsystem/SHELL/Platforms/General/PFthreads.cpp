#include "PFthreads_impl.h"
#include "PFthreads.h"

#include "Universal_System/Resources/resource_data.h" //script_execute

using enigma::ethread;
using enigma::threads;
using enigma::scrtdata;

namespace enigma {

std::deque<ethread*> threads;

void* thread_script_func(void* data) {
  const scrtdata* const md = (scrtdata*)data;
  md->mt->ret = enigma_user::script_execute(md->scr,md->args[0],md->args[1],md->args[2],md->args[3],md->args[4],md->args[5],md->args[6],md->args[7]);
  md->mt->active = false;
  return NULL;
}

} //namespace enigma

namespace enigma_user {

int script_thread(int scr,variant arg0, variant arg1, variant arg2, variant arg3, variant arg4, variant arg5, variant arg6, variant arg7) {
  int thread = thread_create_script(scr,arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7);
  int res = thread_start(thread);
  if (res != 0) {
    thread_delete(thread);
    return res;
  }
  return thread;
}

int thread_create_script(int scr,variant arg0, variant arg1, variant arg2, variant arg3, variant arg4, variant arg5, variant arg6, variant arg7) {
  ethread* newthread = new ethread();
  variant args[] = {arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7};
  newthread->sd = new scrtdata(scr, args, newthread);
  threads.push_back(newthread);
  return threads.size() - 1;
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

} //namespace enigma_user
