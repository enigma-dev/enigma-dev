/** Copyright (C) 2013-2014 Robert B. Colton
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
#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSmatrix.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace enigma {
    glm::mat4 world, view, projection;
}

namespace enigma_user
{

void d3d_set_perspective(bool enable)
{

}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup)
{
    enigma::projection = glm::lookAt(glm::vec3(xfrom, yfrom, zfrom), glm::vec3(xto, yto, zto), glm::vec3(xup, yup, zup));
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
{

}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
    enigma::projection = glm::ortho(x, x + width, y + height, y, 0.1f, 32000.0f);
    enigma::projection = glm::rotate(enigma::projection, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
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

void d3d_transform_add_rotation_x(gs_scalar angle)
{

}

void d3d_transform_add_rotation_y(gs_scalar angle)
{

}

void d3d_transform_add_rotation_z(gs_scalar angle)
{

}

void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{

}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{

}

void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{

}

void d3d_transform_set_rotation_x(gs_scalar angle)
{

}

void d3d_transform_set_rotation_y(gs_scalar angle)
{

}

void d3d_transform_set_rotation_z(gs_scalar angle)
{

}

void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{

}

}

#include <stack>

namespace {

stack<bool> trans_stack;
int trans_stack_size = 0;

}


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

bool d3d_transform_stack_discard()
{
    trans_stack.push(0);
    return true;
}

}
