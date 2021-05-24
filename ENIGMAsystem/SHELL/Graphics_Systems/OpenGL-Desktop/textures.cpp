/** Copyright (C) 2008-2013, Josh Ventura
*** Copyright (C) 2013-2014,2019 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "Graphics_Systems/General/GSprimitives.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/OpenGL-Common/textures_impl.h"
#include "Universal_System/image_formats.h"
namespace enigma{
	
	
	

//added from opengl-common


void graphics_push_texture_pixels(int texture, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
}


int graphics_create_texture(const RawImage& img, bool mipmap, unsigned* fullwidth, unsigned* fullheight)
{
  return graphics_create_texture_custom(img, mipmap, fullwidth, fullheight, GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE);
}


void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, unsigned char* pxdata) {
  glBindTexture(GL_TEXTURE_2D, get_texture_peer(texture));

  glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pxdata);
}



	
	
	
	
} //namespace enigma

namespace enigma_user {

void texture_set_priority(int texid, double prio)
{
  draw_batch_flush(batch_flush_deferred);
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
  // This will give a deprecation message only if called on newer contexts (e.g, GL3+).
  glBindTexture(GL_TEXTURE_2D, enigma::get_texture_peer(texid));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, prio);
}

bool texture_mipmapping_supported()
{
  return GLEW_ARB_framebuffer_object || GLEW_VERSION_3_0;
}

bool texture_anisotropy_supported()
{
  return GLEW_EXT_texture_filter_anisotropic;
}

float texture_anisotropy_maxlevel()
{
  float maximumAnisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
  return maximumAnisotropy;
}



}//namespace enigma_user
