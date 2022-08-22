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


#ifndef ENIGMA_GM5COMPAT_DRAWING_H
#define ENIGMA_GM5COMPAT_DRAWING_H


///This file contains drawing functions in the GM5 style.
///GM5 always draws shapes as background+pen, with the pen having
/// a variable thickness. The background can be "hollow", "solid",
/// or several other patterns.
///Note that drawing with these functions may incur some slowdown,
/// due to the way that line widths are simulated.


#include "Universal_System/var4.h"
#include "Universal_System/scalar.h"

#include "bind.h"


namespace enigma_user
{

//Placing these here allows Enigma to identify them as globals, so
// they can be set properly from any script.
//BUG: Invalid statements such as "brush_color = dafadf;" seem to be valid.
extern gs_scalar pen_size;
extern int brush_style;
extern int pen_color;
extern enigma::BindPropRW brush_color; //We choose to map the brush color to "draw_get/set_color()".


//Brush styles.
enum {
  bs_solid  = 0,
  bs_hollow,

  //Currently not supported.
  bs_bdiagonal,
  bs_fdiagonal,
  bs_cross,
  bs_diagcross,
  bs_horizontal,
  bs_vertical,
};


//Drawing - Wrappers.
void draw_rectangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2);
void draw_triangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3);
void draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2);
void draw_circle(gs_scalar x, gs_scalar y, float radius);


//Drawing - Polygons
void draw_polygon_begin();
void draw_polygon_vertex(gs_scalar x, gs_scalar y, int color=-1);
void draw_polygon_end(bool outline, bool allowHoles=true);
void draw_polygon_end();

}

#endif // ENIGMA_GM5COMPAT_DRAWING_H
