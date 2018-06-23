#include "ASYNCdialog.h"

#include "Platforms/General/PFthreads.h"

#include "Universal_System/Extensions/DataStructures/include.h"

using enigma::threads;
using enigma_user::async_load;
using enigma_user::ds_map_exists;
using enigma_user::ds_map_create;

namespace enigma {

struct fn {
  void* (*fnc)(void*);
  void* data;
};


int _thread_func(void* data) {
  fn* f = (fn*)data;
  f->fnc(f->data);
  return 0;
}

int createThread(void* (*fnc)(void*), MessageData* md) {
  //TODO: According to Studio's manual each time these async functions are called
  //they get their own map and async_load is only set to that map when the async dialog
  //event is fired.
  //This is inferred from the get_login_async documentation.
  if (!ds_map_exists(async_load)) async_load = ds_map_create();

  ethread* newthread = new ethread();
  md->id = threads.size();

  fn* f = new fn();
  f->fnc = fnc;
  f->data = md;

  newthread->handle = SDL_CreateThread(&_thread_func, NULL, (void*)f);

  if (newthread->handle == NULL) {
    delete md; delete newthread;
    return -1;
  }
  
  threads.push_back(newthread);
  return threads.size() - 1;
}

} //namespace enigma
