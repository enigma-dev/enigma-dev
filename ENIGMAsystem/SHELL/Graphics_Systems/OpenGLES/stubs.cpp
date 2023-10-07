#include "Platforms/General/PFwindow.h"

namespace enigma {

void msaa_fbo_blit() {}
void gl_screen_init() {}
void init_vao() {}
void graphics_flush_ext() {}

} // namespace enigma

namespace enigma_user {

void display_reset(int samples, bool vsync) {
  set_synchronization(vsync);
}

void texture_set_priority(int texid, double prio) {}
bool texture_mipmapping_supported() { return false; }
bool texture_anisotropy_supported() { return false; }
float texture_anisotropy_maxlevel() { return 0; }
bool gl_extension_supported(std::string extension) { return false; }
void graphics_state_flush_extra() {}
int draw_get_msaa_maxlevel() { return 0; }
bool draw_get_msaa_supported() { return false; }
bool surface_is_supported() { return false; }
int surface_create(int width, int height, bool depthbuffer, bool stencilbuffer, bool writeonly) { return -1; }
int surface_create_msaa(int width, int height, int samples) { return -1; }

} // namespace enigma_user
