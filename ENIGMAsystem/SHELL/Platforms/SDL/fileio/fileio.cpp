#include "Platforms/General/fileio.h"

#include <SDL2/SDL.h>

size_t fread_wrapper(void* ptr, size_t size, size_t maxnum, FILE_t* context) { 
  return SDL_RWread(context, ptr, size, maxnum);
}

FILE_t* fopen_wrapper(const char* fname, const char* mode) {
  return SDL_RWFromFile(fname, mode);
}

FILE_t* freopen_wrapper(const char* fname, const char* mode, FILE_t* context) {
  return (SDL_RWclose(context) + SDL_RWFromFile(fname, mode));
}

int64_t fseek_wrapper(FILE_t* context, int64_t offset, int whence) {
  return SDL_RWseek(context, offset, whence);
}

int fclose_wrapper(FILE_t* context) {
  return SDL_RWclose(context);
}

int fgetc_wrapper(FILE_t* context) {
 int c;
 SDL_RWread(context, &c, sizeof(char), 1);
 return c; 
}

char* fgets_wrapper(char* str, int num, FILE_t* context) {
  if (!SDL_RWread(context, str, sizeof(char), num)) {
    return nullptr;
  }
  return str;
}

int fputc_wrapper(int character, FILE_t* context) {
 return SDL_RWwrite(context, &character, sizeof(char), 1); 
}

int feof_wrapper(FILE_t* context) {
  return !SDL_RWread(context, nullptr, sizeof(char), 1);
}

int64_t ftell_wrapper(FILE_t* context) {
  return SDL_RWtell(context);
}

size_t fwrite_wrapper(const void *ptr, size_t size, size_t count, FILE_t* context) {
  return SDL_RWwrite(context, ptr, size, count);
}
