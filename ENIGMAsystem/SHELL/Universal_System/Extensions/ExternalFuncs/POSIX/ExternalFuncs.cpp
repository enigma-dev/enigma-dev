#include "Universal_System/Extensions/ExternalFuncs/ExternalFuncs.h"
#include <dlfcn.h>

namespace enigma {

void *ExternalLoad(const char *dll) {
  return dlopen(dll, RTLD_LAZY);
}

bool ExternalFree(void *handle) {
  return (dlclose(handle) == 0);
}

void *ExternalFunc(void *handle, const char *func) {
  return dlsym(handle, func);
}

} // namespace enigma
