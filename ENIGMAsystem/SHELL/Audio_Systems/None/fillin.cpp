#include <cstdlib>

namespace enigma
{
  int audiosystem_initialize() { return 0; }
  void audiosystem_update() {}
  int sound_add_from_buffer(int id, void* buffer, size_t size) { return -1; }
  void audiosystem_cleanup() {}
}
