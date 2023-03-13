#include "Platforms/General/fileio.h"

size_t fread_wrapper(void* ptr, size_t size, size_t maxnum, FILE_t* context) { 
  return fread(ptr, size, maxnum, context);
}

FILE_t* fopen_wrapper(const char* fname, const char* mode) {
  return fopen(fname, mode);
}

FILE_t* freopen_wrapper(const char* fname, const char* mode, FILE_t* context) {
  return freopen(fname, mode, context);
}

int64_t fseek_wrapper(FILE_t* context, int64_t offset, int whence) {
  return fseek(context, offset, whence);
}

int fclose_wrapper(FILE_t* context) {
  return fclose(context);
}

int fgetc_wrapper(FILE_t* context) {
 return fgetc(context); 
}

char* fgets_wrapper(char* str, int num, FILE_t* context) {
  return fgets(str, num, context);
}

int fputc_wrapper(int character, FILE_t* context) {
 return fputc(character, context);
}

int feof_wrapper(FILE_t* context) {
  return feof(context);
}

int64_t ftell_wrapper(FILE_t* context) {
  return ftell(context);
}

size_t fwrite_wrapper(const void *ptr, size_t size, size_t count, FILE_t* context) {
  return fwrite(ptr, size, count, context);
}
