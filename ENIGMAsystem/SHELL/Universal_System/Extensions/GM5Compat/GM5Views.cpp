//
// Copyright (C) 2014 Seth N. Hetu
//
// This file is a part of the ENIGMA Development Environment.
//
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
//
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//


#include "GM5Views.h"

#include "Universal_System/roomsystem.h"


namespace enigma_user
{

enigma::BindArrayRW view_x(view_xport);
enigma::BindArrayRW view_y(view_yport);
enigma::BindArrayRO view_width(view_wview);
enigma::BindArrayRO view_height(view_hview);
enigma::BindArrayRW view_left(view_xview);
enigma::BindArrayRW view_top(view_yview);

}
