#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

void *ExternalLoad(const char *dll);
bool ExternalFree(void *handle);
void *ExternalFunc(void *handle, const char *func);
