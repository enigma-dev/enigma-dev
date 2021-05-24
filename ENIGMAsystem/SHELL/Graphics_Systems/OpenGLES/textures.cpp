#include "Graphics_Systems/OpenGL-Common/textures_impl.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include <cstring> 
#include "Platforms/General/PFwindow.h"


namespace enigma{


void graphics_push_texture_pixels(int texture, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxdata);
}


int graphics_create_texture(const RawImage& img, bool mipmap, unsigned* fullwidth, unsigned* fullheight)
{
  return graphics_create_texture_custom(img, mipmap, fullwidth, fullheight, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
}


void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pxdata);
}





}// namespace enigma