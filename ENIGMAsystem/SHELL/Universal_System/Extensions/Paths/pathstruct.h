/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
**  Modified 2013 by Josh Ventura                                               **
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

#include <map>
#include <vector>
using std::vector;
using std::map;

#include "Universal_System/scalar.h"

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

namespace enigma
{
  struct path_point
  {
    cs_scalar x, y, speed, length;
    path_point(cs_scalar X = 0, cs_scalar Y = 0, cs_scalar Speed = 0, cs_scalar Length = 0):
      x(X), y(Y), speed(Speed), length(Length) {}
  };
  struct path
  {
    int id, precision;
    bool smooth, closed;
    vector<path_point> pointarray;
    map<cs_scalar,int> pointoffset;
    cs_scalar total_length, centerx, centery;
    path(unsigned pathid, bool smooth, bool closed, int precision, unsigned pointcount);
    ~path();
  };

  extern path** pathstructarray;
  void path_add_point(unsigned pathid, cs_scalar x, cs_scalar y, cs_scalar speed);
  void path_recalculate(unsigned pathid);
  void path_getXY(path *pth, cs_scalar &x, cs_scalar &y, cs_scalar position);
  void path_getspeed(path *pth, cs_scalar &speed, cs_scalar position);
  void pathstructarray_reallocate();
  typedef map<cs_scalar,int>::iterator ppi_t;
}

namespace enigma
{
	//Allocates and zero-fills the array at game start
	void paths_init();
}
