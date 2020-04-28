namespace enigma {

void *ExternalLoad(const char *dll);
bool ExternalFree(void *handle);
void *ExternalFunc(void *handle, const char *func);

}
