/** Copyright (C) 2021 Robert Colton
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

#include "VKtextures_impl.h"
#include "VulkanHeaders.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"

using namespace enigma::vlkn;

namespace enigma {

int graphics_create_texture(const RawImage& img, bool mipmap, unsigned* fullwidth, unsigned* fullheight) {
  return -1; //TODO: implement
}

void graphics_delete_texture(int tex) {

}

unsigned char* graphics_copy_texture_pixels(int texture, unsigned* fullwidth, unsigned* fullheight) {
  return nullptr;
}

unsigned char* graphics_copy_texture_pixels(int texture, int x, int y, int width, int height) {
  unsigned fw, fh;
  unsigned char* pxdata = graphics_copy_texture_pixels(texture, &fw, &fh);
  return pxdata;
}

void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, unsigned char* pxdata) {}

void graphics_push_texture_pixels(int texture, int width, int height, unsigned char* pxdata) {}

} // namespace enigma

namespace enigma_user {

void texture_set_priority(int texid, double prio)
{
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
}

bool texture_mipmapping_supported()
{
  return false; //TODO: implement
}

bool texture_anisotropy_supported()
{
  return false; //TODO: implement
}

float texture_anisotropy_maxlevel()
{
  return 0.0f; //TODO: implement
}

} // namespace enigma_user
