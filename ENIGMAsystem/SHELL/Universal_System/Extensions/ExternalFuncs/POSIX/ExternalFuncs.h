#include <dlfcn.h>

#define ExternalLoad(x) dlopen(x, RTLD_LAZY)
#define ExternalFree(x) (dlclose(x) == 0)
