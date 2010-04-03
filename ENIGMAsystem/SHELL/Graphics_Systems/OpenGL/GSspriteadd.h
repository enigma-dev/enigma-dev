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

/**Sprite adding functions*****************************************************\

int sprite_add(std::string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset)

\******************************************************************************/

//#include "../../Universal_System/spritestruct.h"

int sprite_add(std::string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset);

/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */

namespace enigma
{
  //Adds an empty sprite to the list
  int new_sprexe(int sprid,int w,int h,int x,int y,int pl,int sm);
  
  #if COLLIGMA
  collCustom* generate_bitmask(unsigned char* pixdata,int x,int y,int w,int h);
  #endif
  
  //Adds a subimage to an existing sprite from the exe
  void sprexe(int sprid,
    #if COLLIGMA
    int x,int y,
    #endif
    unsigned int w,unsigned int h,unsigned char*tra,unsigned char*chunk);
}
