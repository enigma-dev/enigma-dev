/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012 forthevin                                                **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "PS_particle_sprites.h"
#include "PS_particle_enums.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include <map>
#include <cmath>

namespace enigma
{
  std::map<pt_shape,particle_sprite*> shape_to_sprite;
  void generate_sphere()
  {
    //TODO: Implement correctly.
    int fullwidth = 64, fullheight = 64; // Assumed to be equal to n^2 for some n.
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    int center_x = fullwidth/2, center_y = fullheight/2;

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double dist_from_middle = sqrt(x_d*x_d + y_d*y_d);
        int grayvalue = 255;
        int alpha = dist_from_middle > 32 ? 0 : int(255*(32 - dist_from_middle)/32);
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    unsigned texture = graphics_create_texture(fullwidth,fullheight,imgpxdata);
    delete[] imgpxdata;

    particle_sprite* p_sprite = new particle_sprite();
    p_sprite->texture = texture;
    p_sprite->width = fullwidth;
    p_sprite->height = fullheight;

    shape_to_sprite.insert(std::pair<pt_shape,particle_sprite*>(pt_shape_sphere,p_sprite));
  }
  void initialize_particle_sprites()
  {
    generate_sphere();
  }
}
 
