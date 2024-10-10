#include <cstdlib>

#include "include.h"

namespace enigma
{
  int audiosystem_initialize() { return 0; }
  void audiosystem_update() {}
  int sound_add_from_buffer(int id, void* buffer, size_t size) { return -1; }
  void audiosystem_cleanup() {}
}

namespace enigma_user
{
  int sound_add(string fname, int kind, bool preload) {
    return -1;
  }

  void sound_pitch(int sound, float value) {}
}
