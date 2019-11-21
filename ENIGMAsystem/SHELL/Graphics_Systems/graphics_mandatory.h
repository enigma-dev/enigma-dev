/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

/*\\\ This file contains prototypes for functions that must be defined by the graphics
|*||| library wrapper modules. Each of these is used by other systems throughout the engine.
\*/// Accidental failure to implement them could cause error.

#include "Universal_System/Extensions/ParticleSystems/PS_particle_instance.h"
#include <string>
#include <vector>

namespace enigma
{
  /// Called at game load to allow the system to set up.
  void graphicssystem_initialize(); /// This function can be implemented as an empty call if it is not needed.

  /// Generate a texture from image data. Preserves input pixbuf.
  int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap = false);
  int graphics_duplicate_texture(int tex, bool mipmap=false);
  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex);
  void graphics_delete_texture(int tex);
  void graphics_copy_texture(int source, int destination, int x, int y); //Copy one into another with position offset x,y
  void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y); //Copy rectangle [xoff,yoff,xoff+w,yoff+h] from source to [x,y] in destination

  /// Retrieve image data from a texture, in unsigned char, BGRA format.
  /// This data will be allocated afresh; the pointer and data are yours to manipulate
  /// and must be freed once you are done.
  unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight);

  struct particles_implementation
  {
    // Draws all given particle systems in the depth range [-high, -low[ that are automatically drawn.
    void (*draw_particlesystems)(double high, double low);
  };
  void set_particles_implementation(particles_implementation* particles_impl);
  void delete_tiles();
  void load_tiles();
}
// These functions are available to the user to be called on a whim.

namespace enigma_user
{
  // Called at random. Maybe.
  std::string draw_get_graphics_error(); // Return a const char* error string, if any error has occurred, or an empty string otherwise.

  // Called each step, or by the user randomly.
  void screen_refresh(); // Without invoking any events, refresh the contents of the screen.

  // Called each step, or by the user randomly.
  void screen_redraw(); // Invoke all the draw events, but do not refresh the screen.

  void screen_init();

  // Called by the room system to retrieve dimensions for background_width/height[room background index].
  int background_get_width(int backId);
  int background_get_height(int backId);

  //For game settings
  void texture_set_interpolation(bool enable);
  void texture_set_interpolation_ext(int sampler, bool enable);
  #define texture_set_interpolation(enable) texture_set_interpolation_ext(0, enable)
}

