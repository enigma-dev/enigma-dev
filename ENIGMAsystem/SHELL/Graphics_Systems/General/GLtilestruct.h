/** Copyright (C) 2008-2013 polygone
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

#include "Universal_System/roomsystem.h"
namespace enigma
{
    struct bkinxop
    {
        bool operator() (const tile a, const tile b) {return (a.bckid < b.bckid);}
    }   bkinxcomp;
    void draw_tile();
    void delete_tiles();
    void load_tiles();
    void rebuild_tile_layer(int layer_depth);
}
