/** Copyright (C) 2008-2018 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton, TheExDeus
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

#include "var4.h"
#include "dynamic_args.h"

#include <limits>
#include <algorithm> // min/max
#include <stdlib.h>
#include <cmath>

// Note: This hack is justifiable in that it was put here to prevent
// around contributors' bad habits, not because of developers' bad habits.
#define INCLUDED_FROM_SHELLMAIN Not really.
#include "mathnc.h"
#undef INCLUDED_FROM_SHELLMAIN

#define M_PI    3.14159265358979323846

//overloading

namespace enigma_user
{
  ma_scalar triangle_area(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3){
    return fabs(((x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1))/2.0);
  }

  bool point_in_circle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar rad) {
    return (((px-x1)*(px-x1)+(py-y1)*(py-y1))<rad*rad);
  }

  bool point_in_rectangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2) {
    return px >= x1 && px <= x2 && py >= y1 && py <= y2;
  }

  bool point_in_triangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3) {
    ma_scalar a = (x1 - px)*(y2 - py) - (x2 - px)*(y1 - py);
    ma_scalar b = (x2 - px)*(y3 - py) - (x3 - px)*(y2 - py);
    ma_scalar c = (x3 - px)*(y1 - py) - (x1 - px)*(y3 - py);
    return (sign(a) == sign(b) && sign(b) == sign(c));
  }

  //Based on GMLscripts.com
  ma_scalar lines_intersect(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3, ma_scalar x4, ma_scalar y4, bool segment) {
    ma_scalar ud, ua = 0;
    ud = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (ud != 0) {
        ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ud;
        if (segment) {
            ma_scalar ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ud;
            if (ua < 0 || ua > 1 || ub < 0 || ub > 1) ua = 0;
        }
    }
    return ua;
  }

   int rectangle_in_rectangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar dx1, ma_scalar dy1, ma_scalar dx2, ma_scalar dy2) {
    if (dx2 > sx1 && dx1 < sx2 && dy2 > sy1 && dy1 < sy2) {
      ma_scalar iw = std::min(sx2, dx2) - std::max(sx1, dx1);
      ma_scalar ih = std::min(sy2, dy2) - std::max(sy1, dy1);
      return (iw*ih==std::abs((sx2-sx1)*(sy2-sy1))?1:2);
    }
    return 0;
  }

  int rectangle_in_circle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar x, ma_scalar y, ma_scalar rad){
    ma_scalar rw2 = (sx2-sx1)/2.0;
    ma_scalar rh2 = (sy2-sy1)/2.0;

    ma_scalar cDx = fabs(x - (sx1+rw2));
    ma_scalar cDy = fabs(y - (sy1+rh2));

    rw2 = fabs(rw2);
    rh2 = fabs(rh2);

    //Check no intersection
    if (cDx > (rw2 + rad)) return 0;
    if (cDy > (rh2 + rad)) return 0;

    //Check if totally inside
    ma_scalar cpx = fmax(fabs(x-sx1),fabs(x-sx2));
    ma_scalar cpy = fmax(fabs(y-sy1),fabs(y-sy2));
    if (cpx*cpx + cpy*cpy <= rad*rad) return 1;

    //Check partial overlap
    if (cDx <= rw2 ) return 2;
    if (cDy <= rh2 ) return 2;
    //Check corner case
    ma_scalar csq = (cDx - rw2)*(cDx - rw2) + (cDy - rh2)*(cDy - rh2);
    return ((csq <= (rad*rad))?2:0);
  }

  //Based on discussion here in http://seb.ly/2009/05/super-fast-trianglerectangle-intersection-test/
  int rectangle_in_triangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3){
    //Check if all points of the triangle are on one side of the rectangle
    if ((sx1>x1 && sx1>x2 && sx1>x3) || (sx2<x1 && sx2<x2 && sx2<x3) || (sy1>y1 && sy1>y2 && sy1>y3) || (sy2<y1 && sy2<y2 && sy2<y3)){
         return 0;
    }

    //Check partial collision with lines
    int b1 = ((x1 > sx1) ? 1 : 0) | (((y1 > sy1) ? 1 : 0) << 1) |
        (((x1 > sx2) ? 1 : 0) << 2) | (((y1 > sy2) ? 1 : 0) << 3);
    if (b1 == 3) return 2;

    int b2 = ((x2 > sx1) ? 1 : 0) | (((y2 > sy1) ? 1 : 0) << 1) |
        (((x2 > sx2) ? 1 : 0) << 2) | (((y2 > sy2) ? 1 : 0) << 3);
    if (b2 == 3) return 2;

    int b3 = ((x3 > sx1) ? 1 : 0) | (((y3 > sy1) ? 1 : 0) << 1) |
        (((x3 > sx2) ? 1 : 0) << 2) | (((y3 > sy2) ? 1 : 0) << 3);
    if (b3 == 3) return 2;

    //Check partial collision with points
    int i = b1 ^ b2;
    if (i != 0)
    {
        ma_scalar c, m, s;
        m = (y2-y1) / (x2-x1);
        c = y1 -(m * x1);
        if (i & 1) { s = m * sx1 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 2) { s = (sy1 - c) / m; if ( s > sx1 && s < sx2) return 2; }
        if (i & 4) { s = m * sx2 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 8) { s = (sy2 - c) / m; if ( s > sx1 && s < sx2) return 2; }
    }

    i = b2 ^ b3;
    if (i != 0)
    {
        ma_scalar c, m, s;
        m = (y3-y2) / (x3-x2);
        c = y2 -(m * x2);
        if (i & 1) { s = m * sx1 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 2) { s = (sy1 - c) / m; if ( s > sx1 && s < sx2) return 2; }
        if (i & 4) { s = m * sx2 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 8) { s = (sy2 - c) / m; if ( s > sx1 && s < sx2) return 2; }
    }

    i = b1 ^ b3;
    if (i != 0)
    {
        ma_scalar c, m, s;
        m = (y3-y1) / (x3-x1);
        c = y1 -(m * x1);
        if (i & 1) { s = m * sx1 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 2) { s = (sy1 - c) / m; if ( s > sx1 && s < sx2) return 2; }
        if (i & 4) { s = m * sx2 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 8) { s = (sy2 - c) / m; if ( s > sx1 && s < sx2) return 2; }
    }

    //Check if totally overlapped (at this point can be done by checking if the rectangle center is inside the triangle)
    if (point_in_triangle(sx1+(sx2-sx1)/2.0, sy1+(sy2-sy1)/2.0,x1,y1,x2,y2,x3,y3)) return 1;

    return 0;
  }

  int ray_sphere_intersect(ma_scalar xc, ma_scalar yc, ma_scalar zc, ma_scalar xs, ma_scalar ys, ma_scalar zs, ma_scalar xd, ma_scalar yd, ma_scalar zd, ma_scalar r){
    ma_scalar b = 2*(xd*(xs-xc)+yd*(ys-yc)+zd*(zs-zc));
    ma_scalar c = xs*xs - 2*xs*xc+xc*xc+ys*ys - 2*ys*yc+yc*yc+zs*zs - 2*zs*zc+zc*zc-r*r;
    ma_scalar det = (b*b-4*c);
    if (det < 0.0) return 0;
    else if (equal(det, 0.0) == 1) return 1;
    else return 2;
  }


  ma_scalar dot_product(ma_scalar x1,ma_scalar y1,ma_scalar x2,ma_scalar y2) { return (x1 * x2 + y1 * y2); }
  ma_scalar dot_product_3d(ma_scalar x1,ma_scalar y1,ma_scalar z1,ma_scalar x2,ma_scalar y2, ma_scalar z2) { return (x1 * x2 + y1 * y2 + z1 * z2); }
  ma_scalar dot_product_normalised(ma_scalar x1,ma_scalar y1,ma_scalar x2,ma_scalar y2) {
  	ma_scalar length = sqrt(x1*x1+y1*y1);

    x1 = x1/length;
    y1 = y1/length;

	length = sqrt(x2*x2+y2*y2);

    x2 = x2/length;
    y2 = y2/length;
    return (x1 * x2 + y1 * y2);
  }
  ma_scalar dot_product_normalised_3d(ma_scalar x1,ma_scalar y1,ma_scalar z1,ma_scalar x2,ma_scalar y2, ma_scalar z2) {
	ma_scalar length = sqrt(x1*x1+y1*y1+z1*z1);

    x1 = x1/length;
    y1 = y1/length;
    z1 = z1/length;

	length = sqrt(x2*x2+y2*y2+z2*z2);

    x2 = x2/length;
    y2 = y2/length;
    z2 = z2/length;
    return (x1 * x2 + y1 * y2 + z1 * z2);
  }
  ma_scalar lerp(ma_scalar x, ma_scalar y, ma_scalar a) { return std::fma(a, y-x, x); }
  ma_scalar clamp(ma_scalar val, ma_scalar min, ma_scalar max) {
      if (val < min) { return min; }
      if (val > max) { return max; }
      return val;
  }

  ma_scalar min(ma_scalar x, ma_scalar y) { return x < y ? x : y; }
  ma_scalar max(ma_scalar x, ma_scalar y) { return x > y ? x : y; }

  ma_scalar max(const enigma::varargs &t)
  {
    ma_scalar ret = t.get(0), tst;
    for (int i = 1; i < t.argc; i++)
      if ((tst = t.get(i)) > ret)
        ret = tst;
    return ret;
  }

  ma_scalar min(const enigma::varargs &t)
  {
    ma_scalar ret = t.get(0), tst;
    for (int i = 1; i < t.argc; i++)
      if ((tst = t.get(i)) < ret)
        ret = tst;
    return ret;
  }

  ma_scalar median(enigma::varargs t)
  {
    t.sort();
    if (t.argc & 1)
      return t.get(t.argc/2);
    return (t.get(t.argc/2) + t.get(t.argc/2-1)) / 2.;
  }

  ma_scalar mean(const enigma::varargs &t)
  {
    ma_scalar ret = 0;
    for (int i = 0; i < t.argc; i++)
        ret += t.get(i);
    return ret/t.argc;
  }

  evariant choose(const enigma::varargs& args) {
     return args.get(rand() % args.argc);
  }
}
