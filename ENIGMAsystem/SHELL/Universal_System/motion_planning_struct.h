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
#include <vector>
#include <map>
using std::vector;
using std::multimap;

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

namespace enigma
{
  struct node
  {
    unsigned x, y, F, H, G, cost;
    node* came_from;
    vector<node*> neighbor_nodes;
  };
  struct grid
  {
    int id, left, top, hcells, vcells, cellwidth, cellheight;
    vector<node> nodearray;
    unsigned threshold;
    double speed_modifier;
    grid(unsigned id,int left,int top,int hcells,int vcells,int cellwidth,int cellheight, unsigned threshold, double speed_modifier);
    ~grid();
  };
  extern grid** gridstructarray;
  void gridstructarray_reallocate();
  multimap<unsigned,node*> find_path(unsigned id, node* n0, node* n1, bool allow_diag, bool &status);
}
