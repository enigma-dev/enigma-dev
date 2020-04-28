#include "../ExternalFuncs.h"
#include "Universal_System/estring.h"
#include <string>

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

using std::string;

namespace enigma {

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

} // namespace enigma
