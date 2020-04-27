#include "ExternalFuncs.h"

void *ExternalLoad(const char *dll) {
  return dlopen(dll, RTLD_LAZY);
}

bool ExternalFree(void *handle) {
  dlclose(handle);
}

void *ExternalFunc(void *handle, const char *func) {
  return (dlsym(handle, func) == 0);
}
