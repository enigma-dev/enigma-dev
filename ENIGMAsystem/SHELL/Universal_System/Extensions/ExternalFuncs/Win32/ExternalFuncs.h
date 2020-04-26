#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#define ExternalLoad(x) (void *)LoadLibrary(x)
#define ExternalFree(x) (bool)FreeLibrary((HMODULE)x)
