/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

/*\\\ This file contains prototypes for functions that must be defined by the graphics
|*||| library wrapper modules. Each of these is used by other systems throughout the engine.
\*/// Accidental failure to implement them could cause error.

namespace enigma
{
  // Called at game load to allow the system to set up.
  void graphicssystem_initialize(); // This function can be implemented as an empty call if it is not needed.
  
  // Called at game start if no resource data can be loaded.
  void sprite_safety_override(); // This function should ensure a reasonable number of sprite indexes won't segfault.
  
  // Called at game start.
  void sprites_allocate_initial(int); // This should allocate a certain number of sprites.
  
  //Adds an empty sprite to the list, presumably to be loaded from the exe.
  int sprite_new_empty(unsigned sprid,unsigned subc,int w,int h,int x,int y,int pl,int sm); // Called at load time.
  
  //Adds a subimage to an existing sprite from the exe
  void sprite_add_subimage(int sprid, int x, int y, unsigned int w, unsigned int h, unsigned char* chunk);  // Called at load time.
  
  #if COLLIGMA // FIXME: This doesn't belong here.
  collCustom* generate_bitmask(unsigned char* pixdata,int x,int y,int w,int h);
  #endif
}

// Called at random. Maybe.
const char* draw_get_graphics_error(); // Return a const char* error string, if any
// error has occurred, or an empty string otherwise.
