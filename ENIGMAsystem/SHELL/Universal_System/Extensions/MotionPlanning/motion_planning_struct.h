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

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifdef COLLISION_NONE
	#error The motion planning extension requires a collision system.
#endif

#ifndef PATH_EXT_SET
	#error The motion planning extension requires the paths extension.
#endif

#include <vector>
#include <map>
#include <cstddef>


using std::vector;
using std::multimap;

namespace enigma
{
  struct node
  {
    unsigned x, y, F, H, G, cost;
    node* came_from;
    vector<node*> neighbor_nodes;
    node(unsigned X = 0, unsigned Y = 0, unsigned f = 0, unsigned h = 0, unsigned g = 0, unsigned Cost = 0, node* CameFrom = NULL):
      x(X), y(Y), F(f), H(h), G(g), cost(Cost), came_from(CameFrom) {}
  };
  struct grid
  {
    unsigned int id;
    int left, top;
    unsigned int hcells, vcells, cellwidth, cellheight;
    unsigned threshold;
    double speed_modifier;
    vector<node> nodearray;
    grid(unsigned int id,int left,int top,unsigned int hcells,unsigned int vcells,unsigned int cellwidth,unsigned int cellheight, unsigned int threshold, double speed_modifier);
    ~grid();
  };
  extern grid** gridstructarray;
  void gridstructarray_reallocate();
  multimap<unsigned,node*> find_path(unsigned id, node* n0, node* n1, bool allow_diag, bool &status);
}
