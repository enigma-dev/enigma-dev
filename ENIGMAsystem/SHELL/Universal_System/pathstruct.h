/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
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
//#include "instance_system.h" //for path_inst struct JOSH DO THIS BETTER LATER!
using std::vector;
using std::map;

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

namespace enigma
{
  struct path_point
  {
    double x, y, speed, length;
  };
  struct path
  {
    int id, precision;
    bool smooth, closed;
    vector<path_point> pointarray;
    map<double,int> pointoffset;
    double total_length, centerx, centery;
    path(unsigned pathid, bool smooth, bool closed, int precision, unsigned pointcount);
    ~path();
  };

  /*struct path_inst
  {
    enigma::object_graphics inst;
    unsigned path_index, path_endaction;
    double path_position, path_speed;
    bool absolute;
  };*/

  extern path** pathstructarray;
	void path_add_point(unsigned pathid, double x, double y, double speed);
  void path_recalculate(unsigned pathid);
  void path_getXY(path *pth, double &x, double &y, double position);
  void path_getspeed(path *pth, double &speed, double position);
  void pathstructarray_reallocate();
  typedef map<double,int>::iterator ppi_t;
}

namespace enigma
{
	//Allocates and zero-fills the array at game start
	void paths_init();
}
