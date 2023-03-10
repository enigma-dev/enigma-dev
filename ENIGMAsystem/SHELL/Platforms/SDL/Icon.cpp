/** Copyright (C) 2020 Samuel Venable
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

#include "Icon.h"

#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/nlpo2.h"

namespace enigma {

void SetIconFromSprite(SDL_Window *window, int ind, unsigned subimg) {
  RawImage img = sprite_get_raw(ind, subimg);
  if (img.pxdata == nullptr) return;
  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)img.pxdata, img.w, img.h, 32, img.w * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_SetWindowIcon(window, surface);
  SDL_FreeSurface(surface);
}

} // namespace enigma
