#include "ASYNCdialog.h"

#include "Platforms/General/PFthreads.h"

#include "Universal_System/Extensions/DataStructures/include.h"

using enigma::threads;
using enigma_user::async_load;
using enigma_user::ds_map_exists;
using enigma_user::ds_map_create;

namespace enigma {

int createThread(void* (*fnc)(void*), MessageData* md) {
  //TODO: According to Studio's manual each time these async functions are called
  //they get their own map and async_load is only set to that map when the async dialog
  //event is fired.
  //This is inferred from the get_login_async documentation.
  if (!ds_map_exists(async_load)) async_load = ds_map_create();

  ethread* newthread = new ethread();
  md->id = threads.size();


  DWORD dwThreadId;
  newthread->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fnc, (LPVOID)md, 0, &dwThreadId);

  //TODO: May need to check if ret is -1L, and yes it is quite obvious the return value is
  //an unsigned integer, but Microsoft says to for some reason. See their documentation here.
  //http://msdn.microsoft.com/en-us/library/kdzttdcb.aspx
  //NOTE: Same issue is in Platforms/General/PFthreads.cpp
  if (newthread->handle == NULL) {
    delete md; delete newthread;
    return -1;
  }
  
  threads.push_back(newthread);
  return threads.size() - 1;
}

} //namespace enigma
