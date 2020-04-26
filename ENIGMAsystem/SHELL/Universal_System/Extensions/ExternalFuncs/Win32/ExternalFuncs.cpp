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
  string str_func = func;
  tstring tstr_func = widen(str_func);
  return (void *)GetProcAddressW((HMODULE)handle, tstr_func.c_str());
}
