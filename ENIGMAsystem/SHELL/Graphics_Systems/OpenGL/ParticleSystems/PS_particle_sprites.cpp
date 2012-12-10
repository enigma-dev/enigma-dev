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
#include <cstdlib>

namespace enigma
{
  std::map<pt_shape,particle_sprite*> shape_to_sprite;

  // Assumes fullwidth == fullheight == 2^n for some n>0.
  void create_particle_sprite(int fullwidth, int fullheight, char* imgpxdata, pt_shape pt_sh)
  {
    unsigned texture = graphics_create_texture(fullwidth,fullheight,imgpxdata);

    particle_sprite* p_sprite = new particle_sprite();
    p_sprite->texture = texture;
    p_sprite->width = fullwidth;
    p_sprite->height = fullheight;

    shape_to_sprite.insert(std::pair<pt_shape,particle_sprite*>(pt_sh,p_sprite));
  }

  void generate_pixel()
  {
    const int fullwidth = 8, fullheight = 8;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        int x_d = abs(x-center_x);
        int y_d = abs(y-center_y);
        int dfm = x_d + y_d; // Distance from middle.
        int grayvalue = 255;
        int alpha = dfm > 2 ? 0 : (dfm == 2 ? 60 : (dfm == 1 ? 140 : 255));
        if ((x_d == 0 && y_d == 2) || (x_d == 2 && y_d == 0)) alpha = 36;
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_pixel);
    delete[] imgpxdata;
  }
  void generate_disk()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int halfwidth = fullwidth/2;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double dfm = sqrt(x_d*x_d + y_d*y_d); // Distance from middle.
        int grayvalue = 255;
        const int in_r = 5, out_r = 3;
        int alpha = dfm >= halfwidth-in_r ? 0 : 255;
        if (dfm >= halfwidth-in_r && dfm <= halfwidth-out_r) {
          alpha = int(255*((halfwidth-out_r) - dfm)/(in_r-out_r));
        }
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_disk);
    delete[] imgpxdata;
  }
  void generate_square()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int halfwidth = fullwidth/2;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = abs(x+0.5-center_x);
        double y_d = abs(y+0.5-center_y);
        double dfm = std::max(x_d, y_d); // Distance from middle.
        int grayvalue = 255;
        const int in_r = 5, out_r = 3;
        int alpha = dfm >= halfwidth-in_r ? 0 : 255;
        if (dfm >= halfwidth-in_r && dfm <= halfwidth-out_r) {
          alpha = int(255*((halfwidth-out_r) - dfm)/(in_r-out_r));
        }
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_square);
    delete[] imgpxdata;
  }
  void generate_line()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int halfwidth = fullwidth/2;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = abs(x+0.5-center_x);
        double y_d = abs(y+0.5-center_y)*6;
        double dfm = std::max(x_d, y_d); // Distance from middle.
        int grayvalue = 255;
        const int in_r = 10, out_r = 4;
        int alpha = dfm >= halfwidth-in_r ? 0 : 255;
        if (dfm >= halfwidth-in_r && dfm <= halfwidth-out_r) { // Rounded corners.
          x_d = std::max(0.0,x_d - (halfwidth-in_r));
          y_d = std::max(0.0,y_d - (halfwidth-in_r));
          dfm = sqrt(x_d*x_d + y_d*y_d);
          alpha = dfm > (in_r-out_r) ? 0 : int(255*((in_r-out_r) - dfm)/(in_r-out_r));
        }
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_line);
    delete[] imgpxdata;
  }
  class V2
  {
    public:
    double x;
    double y;
    explicit V2() : x(0), y(0)
    {
    }
    explicit V2(double x1, double y1) : x(x1), y(y1)
    {
    }
    double dotproduct(const V2& other) const
    {
      return x*other.x + y*other.y;
    }
    double crossproduct(const V2& other) const
    {
      return x*other.y - y*other.x;
    }
    V2 minus(const V2& other) const
    {
      return V2(x - other.x, y - other.y);
    }
    double norm() const
    {
      return sqrt(x*x + y*y);
    }
    V2 unit_vector() const
    {
      const double nor = norm();
      return V2(x/nor, y/nor);
    }
    V2 multiply(double value) const
    {
      return V2(value*x, value*y);
    }
  };
  void generate_star()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    V2 center(center_x, center_y);
    const double rad = 25.0;
    V2 ss[5];
    for (int i = 0; i < 5; i++)
    {
      ss[i] = V2(
        int(center_x + rad*cos(i*2*M_PI/5.0 - M_PI/2)),
        int(center_y + rad*sin(i*2*M_PI/5.0 - M_PI/2))
      );
    }
    const double border_size = 2.0;
    const int grayvalue = 255;
    const int alpha_max = 190;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        // Check whether the point is inside any area of the star.
        // If inside any triple of lines in star with starting indexes (i, i+1, i+3),
        // point is inside.
        V2 p(x,y);
        bool inside;
        for (int i = 0; i < 5; i++)
        {
          inside = true;
          int is[] = {i, (i+1) % 5, (i+3) % 5};
          for (int e = 0; e < 3; e++)
          {
            V2 v1 = ss[(is[e]+2) % 5].minus(ss[is[e] % 5]);
            V2 v2 = p.minus(ss[is[e] % 5]);
            if (v1.crossproduct(v2) < 0) inside = false; // If on the wrong side, then false.
          }
          if (inside) break;
        }
        int alpha = 0;
        if (inside) { // Fill completely the inner area.
          alpha = alpha_max;
        }
        else { // Fade out the edges.

          // Find minimum distance along line.
          for (int i = 0; i < 5; i++)
          {
            int i_1 = i, i_2 = (i+1) % 5;
            V2 v1 = ss[i_1].minus(center);
            V2 v2 = center.minus(ss[i_2]);
            V2 v3 = p.minus(center);

            if ((v1.crossproduct(v3) >= 0) && (v2.crossproduct(v3) >= 0)) {

              // Find distance to first line.
              V2 u1 = ss[(i_1+2) % 5].minus(ss[i_1]);
              V2 w1 = p.minus(ss[i_1]);
              V2 u1e = u1.unit_vector();
              V2 res1 = u1e.multiply(u1e.dotproduct(w1)).minus(w1);
              double res1s = res1.norm();

              // Find distance to second line.
              V2 u2 = ss[(i_2+3) % 5].minus(ss[i_2]);
              V2 w2 = p.minus(ss[i_2]);
              V2 u2e = u2.unit_vector();
              V2 res2 = u2e.multiply(u2e.dotproduct(w2)).minus(w2);
              double res2s = res2.norm();

              double ress; // Resulting distance.
              // Find distance to first corner, use that if point nearby to give round corner.
              if (w1.dotproduct(u1) <= 0) {
                ress = w1.norm();
              }
              // Find distance to second corner, use that if point nearby to give round corner.
              else if (w2.dotproduct(u2) <= 0) {
                ress = w2.norm();
              }
              else {
                ress = std::min(res1s, res2s);
              }

              alpha = std::max(0, int(alpha_max*(border_size - ress)/border_size));
              break;
            }
          }
        }
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_star);
    delete[] imgpxdata;
  }
  void generate_circle()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int rad = 26;
    const double ring_size = 2.5;
    const int alpha_max = 220;
    const int grayvalue = 255;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double dfm = sqrt(x_d*x_d + y_d*y_d); // Distance from middle.
        double diff = fabs(dfm - rad);
        int alpha = std::max(0, int(alpha_max*(ring_size - diff)/ring_size));
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_circle);
    delete[] imgpxdata;
  }
  void generate_ring()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int out_r = 26;
    const int in_r = 6;
    const double ring_size = 2.5;
    const int alpha_max = 220;
    const int grayvalue = 255;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double dfm = sqrt(x_d*x_d + y_d*y_d); // Distance from middle.
        int alpha = 0;
        if (dfm >= out_r) {
          alpha = std::max(0, int(alpha_max*(ring_size - (dfm-out_r))/ring_size));
        }
        else if (dfm >= in_r) {
          alpha = int(alpha_max*pow(dfm-in_r, 2)/pow(out_r - in_r, 2));
        }
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_ring);
    delete[] imgpxdata;
  }
  void generate_sphere()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int halfwidth = fullwidth/2;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double dfm = sqrt(x_d*x_d + y_d*y_d); // Distance from middle.
        int grayvalue = 255;
        int alpha = dfm > halfwidth ? 0 : int(255*(halfwidth - dfm)/halfwidth);
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_sphere);
    delete[] imgpxdata;
  }
  long random_seed = 0;
  const long M = 2393*12413;
  long get_next_random()
  {
    random_seed = (random_seed*random_seed + 1) % M;
    return random_seed;
  }
  void generate_flare()
  {
    const int fullwidth = 64, fullheight = 64;
    char *imgpxdata = new char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;

    const int line_count = 200;
    const double line_length_min = 26.0;
    const double line_length_max = 26.5;
    double line_lengths[line_count];
    random_seed = 127;
    for (int i = 0; i < line_count; i++)
    {
      line_lengths[i] = line_length_min + (1.0*get_next_random()/M)*(line_length_max-line_length_min);
    }
    for (int i = 0; i < 30; i++) {
      int index = get_next_random() % line_count;
      line_lengths[(index-1) % line_count] = 27.0;
      line_lengths[index] = 28.5;
      line_lengths[(index+1) % line_count] = 27.0;
    }

    int alpha_max = 255;
    int alpha_min = 0;
    const int grayvalue = 255;
    const double dist_max = 29.0;

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double angle = fmod(atan2(y_d, x_d) + 360.0, 360.0);
        int line_index = int(line_count*angle/(2.0*M_PI)) % line_count;
        double dfm = sqrt(x_d*x_d + y_d*y_d) / (line_lengths[line_index]/line_length_min); // Distance from middle.
        int alpha;
        if (dfm >= dist_max) {
          alpha = 0;
        }
        else {
          double part = pow(std::min(1.0, 1.1 - dfm/dist_max), 3);
          alpha = int(alpha_min + part*(alpha_max-alpha_min));
          if (alpha > 255) alpha = 255;
        }
        imgpxdata[4*(x + y*fullwidth)] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, imgpxdata, pt_shape_flare);
    delete[] imgpxdata;
  }
  void initialize_particle_sprites()
  {
    generate_pixel();
    generate_disk();
    generate_square();
    generate_line();
    generate_star();
    generate_circle();
    generate_ring();
    generate_sphere();
    generate_flare();
  }
}
 
