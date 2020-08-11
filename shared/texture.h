#ifndef ENIGMA_SHARED_TEXTURE_H
#define ENIGMA_SHARED_TEXTURE_H

namespace enigma {
struct Texture {
  Texture() : width(0), height(0), channels(1), ID(-1), pxdata(nullptr) {}
  Texture(unsigned width, unsigned height, unsigned char* pxdata, unsigned channels) : width(width), height(height), channels(channels), pxdata(pxdata) {}
  Texture(unsigned width, unsigned height, int ID) : width(width), height(height), channels(1), ID(ID) {}
  void init(); // loads a texture and deletes pxdata (engine only)
  unsigned width;
  unsigned height;
  unsigned channels;
  int ID;
  unsigned char* pxdata;
};
} //namespace enigma

#endif
