/** Copyright (C) 2021 Nabeel Danish 
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

#include "Collision_Systems/collision_mandatory.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/Resources/sprites_internal.h"

#include <iostream>

// Cross product.
static inline double cp(double x1, double y1, double x2, double y2) { return x1*y2 - x2*y1; }
static inline int min(int x, int y) { return x<y? x : y; }
static inline int max(int x, int y) { return x>y? x : y; }

namespace enigma
{
  // A non-NULL pointer is a sprite mask, a NULL pointer means bbox should be used.
  void *get_collision_mask(const Sprite& spr, void* input_data, collision_type ct) // It is called for every subimage of every sprite loaded.
  {
    
    unsigned char* data = (unsigned char*)input_data;
    switch (ct)
    {
      case ct_precise:
        {
          const unsigned int w = spr.width, h = spr.height;
          unsigned char* colldata = new unsigned char[w*h];

          for (unsigned int rowindex = 0; rowindex < h; rowindex++)
          {
            for(unsigned int colindex = 0; colindex < w; colindex++)
            {
              colldata[rowindex*w + colindex] = (data[4*(rowindex*w + colindex) + 3] != 0) ? 1 : 0; // If alpha != 0 then 1 else 0.
            }
          }

          return colldata;
        }
      case ct_bbox: return 0;
      case ct_ellipse:
        {
          // Create ellipse inside bbox.
          const unsigned int w = spr.width, h = spr.height;
          unsigned char* colldata = new unsigned char[w*h](); // Initialize all elements to 0.
          const BoundingBox bbox = spr.bbox;

          const unsigned int a = max(bbox.right()-bbox.left(), bbox.bottom()-bbox.top())/2, // Major radius.
                               b = min(bbox.right()-bbox.left(), bbox.bottom()-bbox.top())/2; // Minor radius.
          const unsigned int xc = (bbox.right()+bbox.left())/2, // Center of ellipse.
                               yc = (bbox.bottom()+bbox.top())/2;

          const long long a_2 = a*a, b_2 = b*b, a_2b_2 = a*a*b*b;

          const int minX = max(bbox.left()-2, 0),
                     minY = max(bbox.top()-2, 0),
                     maxX = min(bbox.right()+2, w),
                     maxY = min(bbox.bottom()+2, h);
          for (int y = minY; y < maxY; y++)
          {
            for(int x = minX; x < maxX; x++)
            {
              const int xcp = x-xc, ycp = y-yc; // Center to point.
              const bool is_inside_ellipse = b_2*xcp*xcp + a_2*ycp*ycp <= a_2b_2;
              colldata[y*w + x] = (is_inside_ellipse ? 1 : 0); // If point inside ellipse, 1, else 0.
            }
          }

          return colldata;
        }
      case ct_diamond:
        {
          // Create diamond inside bbox.
          const unsigned int w = spr.width, h = spr.height;
          unsigned char* colldata = new unsigned char[w*h](); // Initialize all elements to 0.
          const BoundingBox bbox = spr.bbox;

          // Diamond corners.
          const int xl = bbox.left(), yl = (bbox.top() + bbox.bottom())/2,
                               xr = bbox.right(), yr = (bbox.top() + bbox.bottom())/2,
                               xt = (bbox.left() + bbox.right())/2, yt = bbox.top(),
                               xb = (bbox.left() + bbox.right())/2, yb = bbox.bottom();

          const int xlt = xt-xl, ylt = yt-yl, // Left corner to top corner.
                               xlb = xb-xl, ylb = yb-yl, // Left corner to bottom corner.
                               xrt = xt-xr, yrt = yt-yr, // Right corner to top corner.
                               xrb = xb-xr, yrb = yb-yr; // Right corner to bottom corner.

          const int minX = max(bbox.left()-2, 0),
                     minY = max(bbox.top()-2, 0),
                     maxX = min(bbox.right()+2, w),
                     maxY = min(bbox.bottom()+2, h);
          for (int y = minY; y < maxY; y++)
          {
            for(int x = minX; x < maxX; x++)
            {
              const int xlp = x-xl, ylp = y-yl; // Left corner to point.
              const int xrp = x-xr, yrp = y-yr; // Right corner to point.
              const bool is_inside_diamond = cp(xlt, -ylt, xlp, -ylp) <= 0 && // Use cross-product to determine whether inside or outside diamond.
                                              cp(xlb, -ylb, xlp, -ylp) >= 0 &&
                                              cp(xrt, -yrt, xrp, -yrp) >= 0 &&
                                              cp(xrb, -yrb, xrp, -yrp) <= 0;
              colldata[y*w + x] = (is_inside_diamond ? 1 : 0); // If point inside diamond, 1, else 0.
            }
          }

          return colldata;
        }
      case ct_polygon: {
        
        return 0;
      };
      case ct_circle: //NOTE: Not tested.
        {
          // Create circle fitting inside bbox.
          const unsigned int w = spr.width, h = spr.height;
          unsigned char* colldata = new unsigned char[w*h](); // Initialize all elements to 0.
          const BoundingBox bbox = spr.bbox;

          const unsigned int r = min(bbox.right()-bbox.left(), bbox.bottom()-bbox.top())/2; // Radius.
          const unsigned int xc = (bbox.right()+bbox.left())/2, // Center of circle.
                               yc = (bbox.bottom()+bbox.top())/2;

          const int r_2 = r*r;

          const int minX = max(bbox.left()-2, 0),
                     minY = max(bbox.top()-2, 0),
                     maxX = min(bbox.right()+2, w),
                     maxY = min(bbox.bottom()+2, h);
          for (int y = minY; y < maxY; y++)
          {
            for(int x = minX; x < maxX; x++)
            {
              const int xcp = x-xc, ycp = y-yc; // Center to point.
              const bool is_inside_circle = xcp*xcp + ycp*ycp <= r_2;
              colldata[y*w + x] = (is_inside_circle ? 1 : 0); // If point inside circle, 1, else 0.
            }
          }

          return colldata;
        }
      default: return 0;
    };
  }

  void free_collision_mask(void* mask)
  {
    if (mask != 0) {
      delete[] (unsigned char*)mask;
    }
  }
};

