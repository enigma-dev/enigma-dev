#include "Graphics_Systems/OpenGL-Common/textures_impl.h"
//#include "Graphics_Systems/General/GStextures.h"
//#include "Graphics_Systems/General/GStextures_impl.h"
//#include "Graphics_Systems/General/GSprimitives.h"
//#include "Graphics_Systems/graphics_mandatory.h"

namespace enigma {

unsigned char* graphics_copy_texture_pixels(int texture, unsigned* fullwidth, unsigned* fullheight) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));
  *fullwidth = textures[texture]->fullwidth;
  *fullheight = textures[texture]->fullheight;
  unsigned char* ret = new unsigned char[((*fullwidth)*(*fullheight)*4)];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, ret);
  return ret;
}

} //namespace enigma
