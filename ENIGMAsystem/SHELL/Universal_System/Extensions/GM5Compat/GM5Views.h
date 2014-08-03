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


#ifndef ENIGMA_GM5COMPAT_VIEWS__H
#define ENIGMA_GM5COMPAT_VIEWS__H


///This file contains some missing views-based variables for GM5.

#include "Universal_System/var4.h"
#include "bind.h"


namespace enigma_user
{

//View variables that become ambiguous with the introduction of view+port notation.
//Our approach to these is on a case-by-case basis.
extern enigma::BindArrayRW view_x;      //View's X position. Mapped to view_xport
extern enigma::BindArrayRW view_y;      //View's Y position. Mapped to view_yport
extern enigma::BindArrayRO view_width;  //View width. Mapped to view_wview/view_wport
extern enigma::BindArrayRO view_height; //View height. Mapped to view_hview/view_hport
extern enigma::BindArrayRW view_left;   //Left side of the view. Mapped to view_xview
extern enigma::BindArrayRW view_top;    //Top side of the view. Mapped to view_yview


}

#endif // ENIGMA_GM5COMPAT_VIEWS__H

