#include "Graphics_Systems/OpenGL-Common/screen.h"
#include "OpenGLHeaders.h"
#include "Platforms/General/PFwindow.h"

namespace enigma
{
unsigned char* graphics_copy_screen_pixels(int x, int y, int width, int height, bool* flipped) {
  if (flipped) *flipped = true;

  const int bpp = 4; // bytes per pixel
  const int topY = enigma_user::window_get_region_height_scaled()-height-y;
  unsigned char* pxdata = new unsigned char[width*height*bpp];
  GLint prevFbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glReadPixels(x,topY,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pxdata);
  glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
  return pxdata;
}

}	//namespace enigma
