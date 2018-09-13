/** Copyright (C) 2013-2014 Harijs Grinbergs
*** Copyright (C) 2015 Harijs Grinbergs
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

namespace enigma
{
  //Forward declare
  struct Matrix3;
  struct Matrix4;

  extern Matrix4 projection_matrix, view_matrix, model_matrix;
  extern Matrix4 mv_matrix, mvp_matrix;
  extern Matrix3 normal_matrix;
  extern bool transform_needs_update;
  extern void transformation_update();
}
