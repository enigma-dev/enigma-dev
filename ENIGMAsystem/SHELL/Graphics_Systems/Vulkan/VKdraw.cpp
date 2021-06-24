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
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSmatrix_impl.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GSscreen.h"

#include "Universal_System/roomsystem.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

using namespace enigma::vlkn;

namespace {

} // namespace anonymous

namespace enigma {

void graphics_state_flush() {

}

void scene_begin() {}

void scene_end() {}

void graphics_set_viewport(float x, float y, float width, float height) {

}

unsigned char* graphics_copy_screen_pixels(int x, int y, int width, int height, bool* flipped) {

}

unsigned char* graphics_copy_screen_pixels(unsigned* fullwidth, unsigned* fullheight, bool* flipped) {

}

} // namespace enigma

namespace enigma_user {

void draw_clear_alpha(int col, float alpha) {

}

void draw_clear(int col) {

}

int draw_get_msaa_maxlevel() {
  return 0; //TODO: implement
}

bool draw_get_msaa_supported() {
  return false; //TODO: implement
}

void d3d_clear_depth(double value) {

}

void d3d_stencil_clear_value(int value) {

}

void d3d_stencil_clear() {

}

} // namespace enigma_user
