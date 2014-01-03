/** Copyright (C) 2013 Robert B. Colton
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
#include "Bridges/General/DX11Context.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GStextures.h"

#include "../General/GSmodel.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

namespace enigma_user
{

void d3d_set_perspective(bool enable)
{

}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup)
{

}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup,double angle,double aspect,double znear,double zfar)
{

}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle)
{
	
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle)
{

}


// ***** TRANSFORMATIONS BEGIN *****
void d3d_transform_set_identity()
{

}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
	
}

void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{

}

void d3d_transform_add_rotation_x(double angle)
{

}

void d3d_transform_add_rotation_y(double angle)
{

}

void d3d_transform_add_rotation_z(double angle)
{

}

void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
{

}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{

}

void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{

}

void d3d_transform_set_rotation_x(double angle)
{

}

void d3d_transform_set_rotation_y(double angle)
{

}

void d3d_transform_set_rotation_z(double angle)
{

}

void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
{

}

}

#include <stack>
stack<bool> trans_stack;
int trans_stack_size = 0;

namespace enigma_user
{

bool d3d_transform_stack_push()
{

}

bool d3d_transform_stack_pop()
{

}

void d3d_transform_stack_clear()
{

}

bool d3d_transform_stack_empty()
{
    return (trans_stack_size == 0);
}

bool d3d_transform_stack_top()
{

}

bool d3d_transform_stack_disgard()
{
    if (trans_stack_size == 0) return false;
    trans_stack.push(0);
    trans_stack_size--;
    return true;
}

}