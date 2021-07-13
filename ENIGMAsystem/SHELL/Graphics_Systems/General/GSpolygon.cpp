/** Copyright (C) 2013 Robert B. Colton, 2013-2015 Harijs Grinbergs
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

#include "GScolors.h"
#include "GScolor_macros.h"
#include "GSsprite.h"
#include "GStextures.h"
#include "GSprimitives.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/polygon.h"
#include "Universal_System/Resources/polygon_internal.h"

#include <cmath>
#include <cstdlib>
#include <string>

namespace enigma_user {
  void draw_polygon(int polygon_id, gs_scalar x, gs_scalar y) {
    std::vector<enigma::Vector2D> points = enigma::polygons.get(polygon_id).getTransformedPoints();
    enigma::addOffsets(points, x, y);
    int N = points.size();

    // Drawing from point i to i + 1
    for (int i = 0; i < N - 1; ++i) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex(points[i].getX(), points[i].getY());
      draw_vertex(points[i + 1].getX(), points[i + 1].getY());
      draw_primitive_end();
    }

    // Drawing the last line to complete the polygon
    draw_primitive_begin(pr_linestrip);
    draw_vertex(points[N - 1].getX(), points[N - 1].getY());
    draw_vertex(points[0].getX(), points[0].getY());
    draw_primitive_end();
  }
}
