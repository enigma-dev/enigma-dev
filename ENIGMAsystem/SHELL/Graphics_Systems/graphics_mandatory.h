/** Copyright (C) 2008-2011 Josh Ventura
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

namespace enigma
{
  /// Called at game load to allow the system to set up.
  void graphicssystem_initialize(); /// This function can be implemented as an empty call if it is not needed.
  
  /// Called at game start if no resource data can be loaded. //FIXME: This doesn't belong here.
  void sprite_safety_override(); /// This function should ensure a reasonable number of sprite indexes won't segfault.
  
  /// Called at game start. 
  //FIXME: This doesn't belong here.
  void sprites_init(); /// This should allocate room for sprites and perform any other necessary actions.
  
  /// Generate a texture from image data. Preserves input pixbuf.
  unsigned graphics_create_texture(int fullwidth, int fullheight, void* pxdata);
  
  /// Retrieve image data from a texture, in unsigned char, RGBA format.
  /// This data will be allocated afresh; the pointer and data are yours to manipulate
  /// and must be freed once you are done.
  unsigned char* graphics_get_texture_rgba(unsigned texture);
  
  #if COLLIGMA // FIXME: This doesn't belong here.
  collCustom* generate_bitmask(unsigned char* pixdata,int x,int y,int w,int h);
  #endif
}

// These functions are available to the user to be called on a whim.

// Called at random. Maybe.
std::string draw_get_graphics_error(); // Return a const char* error string, if any error has occurred, or an empty string otherwise.

// Called each step, or by the user randomly.
void screen_refresh(); // Without invoking any events, refresh the contents of the screen.

// Called each step, or by the user randomly.
void screen_redraw(); // Invoke all the draw events, but do not refresh the screen.
