#include "ExternalFuncs.h"
#include "Universal_System/estring.h"
#include <string>

using std::string;

void *ExternalLoad(const char *dll) {
  string str_dll = dll;
  tstring tstr_dll = widen(str_dll);
  return (void *)LoadLibraryW(tstr_dll.c_str());
}

bool ExternalFree(void *handle) {
  return (bool)FreeLibrary((HMODULE)handle);
}

void *ExternalFunc(void *handle, const char *func) {
  return (void *)GetProcAddress((HMODULE)handle, func);
}
