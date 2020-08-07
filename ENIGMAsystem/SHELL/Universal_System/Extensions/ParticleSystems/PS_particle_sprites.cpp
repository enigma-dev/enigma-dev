/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012-2013 forthevin                                           **
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

#include <map>
#include <cmath>
#include <cstdlib>

#include <floatcomp.h>

#include "PS_particle_sprites.h"
#include "PS_particle_enums.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Collision_Systems/collision_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h" // show_error
#include "Universal_System/math_consts.h"

#ifdef CODEBLOX
#  define codebloxt(x, y) (x)
#else
#  define codebloxt(x, y) (y)
#endif

namespace enigma
{
  std::map<pt_shape,particle_sprite*> shape_to_sprite;
  std::map<pt_shape,int> shape_to_actual_sprite;

  // Reduce compiler warnings on floats by checking them here; a value of -1 is used to indicate uninitialized indices.
  inline bool negativeone(double x) { return codebloxt((x <= -1 && x >= -1), x == -1); }
  inline bool notnegativeone(double x) { return codebloxt(x < -1 || x > -1,  x != -1); }

  static inline int bounds(int value, int low, int high)
  {
    if (value < low) return low;
    if (value > high) return high;
    return value;
  }
  static inline double fbounds(double value, double low, double high)
  {
    if (value < low) return low;
    if (value > high) return high;
    return value;
  }
  static inline double angle_difference(double dir1, double dir2) {return fmod((fmod((dir1 - dir2),2*M_PI) + 3*M_PI), 2*M_PI) - M_PI;}
  long random_seed = 0;
  const long M = 2393*12413;
  static long get_next_random() // NOTE: Remember to set seed before using.
  {
    random_seed = (random_seed*random_seed + 1) % M;
    return random_seed;
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
    V2 add(const V2& other) const
    {
      return V2(x + other.x, y + other.y);
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

  // Assumes fullwidth == fullheight == 2^n for some n>0.
  static void create_particle_sprite(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, unsigned char* imgpxdata, pt_shape pt_sh)
  {
    unsigned texture = graphics_create_texture(RawImage(imgpxdata, width, height), false, &fullwidth, &fullheight);

    particle_sprite* p_sprite = new particle_sprite();
    p_sprite->texture = texture;
    p_sprite->width = fullwidth;
    p_sprite->height = fullheight;
    p_sprite->shape = pt_sh;

    shape_to_sprite.insert(std::pair<pt_shape,particle_sprite*>(pt_sh,p_sprite));
  }

  static void generate_pixel()
  {
    const int fullwidth = 8, fullheight = 8;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

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
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_pixel);
    delete[] imgpxdata;
  }
  static void generate_disk()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

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
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_disk);
    delete[] imgpxdata;
  }
  static void generate_square()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

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
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_square);
    delete[] imgpxdata;
  }
  static void generate_line()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

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
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_line);
    delete[] imgpxdata;
  }
  static void generate_star()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

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
    const char grayvalue = (char)255;
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
        imgpxdata[4*(x + y*fullwidth)+2]   = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_star);
    delete[] imgpxdata;
  }
  static void generate_circle()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int rad = 26;
    const double ring_size = 2.5;
    const int alpha_max = 220;
    const char grayvalue = (char)255;
    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double dfm = sqrt(x_d*x_d + y_d*y_d); // Distance from middle.
        double diff = fabs(dfm - rad);
        int alpha = std::max(0, int(alpha_max*(ring_size - diff)/ring_size));
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_circle);
    delete[] imgpxdata;
  }
  static void generate_ring()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;
    const int out_r = 26;
    const int in_r = 6;
    const double ring_size = 2.5;
    const int alpha_max = 220;
    const char grayvalue = (char)255;
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
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_ring);
    delete[] imgpxdata;
  }
  static void generate_sphere()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

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
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_sphere);
    delete[] imgpxdata;
  }
  static void generate_flare()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;

    const int line_count = 200;
    const double line_length_min = 26.0;
    const double line_length_max = 26.5;
    double line_lengths[line_count];
    random_seed = 127; // NOTE: Setting random.
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
    const char grayvalue = (char)255;
    const double dist_max = 29.0;

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double angle = fmod(atan2(y_d, x_d) + 2*M_PI, 2*M_PI);
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
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_flare);
    delete[] imgpxdata;
  }
  static void generate_spark()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;

    const int line_count = 200;
    const double line_length_min = 19.0;
    const double line_length_max = 29.0;
    double line_lengths[line_count];
    double line_strengths[line_count];
    random_seed = 211; // NOTE: Setting random.
    for (int i = 0; i < line_count; i++)
    {
      line_lengths[i] = -1;
    }
    for (int i = 0; i < 40; i++) {
      int index = get_next_random() % line_count;
      double length = line_length_min + (line_length_max-line_length_min)*(1.0*get_next_random()/M);
      line_lengths[index] = length;
      line_strengths[index] = 0.8 + (0.4*get_next_random()/M);
    }

    int alpha_max = 255;
    int alpha_min = 20;
    const char grayvalue = (char)255;
    const double line_half_width = 1.5;

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        double x_d = (x+0.5-center_x);
        double y_d = (y+0.5-center_y);
        double angle = fmod(atan2(y_d, x_d) + 2*M_PI, 2*M_PI);
        int line_index = int(round(line_count*angle/(2.0*M_PI))) % line_count;
        int alpha;
        int line_index_low = line_index;
        int line_index_high = line_index;
        while (true)
        {
          if (negativeone(line_lengths[line_index_low])) {
            line_index_low = (line_index_low-1) % line_count;
          }
          if (negativeone(line_lengths[line_index_high])) {
            line_index_high = (line_index_high+1) % line_count;
          }
          if (notnegativeone(line_lengths[line_index_low]) && notnegativeone(line_lengths[line_index_high])) {
            break;
          }
        }
        double dfm = sqrt(x_d*x_d + y_d*y_d); // Distance from middle.
        double angle_diff1 = fabs(angle_difference(2*M_PI*line_index_low/line_count, angle));
        double angle_diff2 = fabs(angle_difference(2*M_PI*line_index_high/line_count, angle));
        double distance_to_line1 = sin(angle_diff1) * dfm * (1 + 2*pow(dfm/line_length_max, 5));
        double distance_to_line2 = sin(angle_diff2) * dfm * (1 + 2*pow(dfm/line_length_max, 5));
        double distance_part1 = fbounds(1.2*(line_half_width - distance_to_line1) / line_half_width, 0.0, 1.0);
        double distance_part2 = fbounds(1.2*(line_half_width - distance_to_line2) / line_half_width, 0.0, 1.0);
        if (dfm >= line_length_max || (distance_part1 <= 0 && distance_part2 <= 0)) {
          alpha = 0;
        }
        else {
          double dfm_mod1 = dfm*line_length_max/line_lengths[line_index_low];
          double part1 = pow(fbounds(1.1 - dfm_mod1/line_length_max, 0.0, 1.0), 2);
          double s1 = line_strengths[line_index_low]*part1*(alpha_max-alpha_min);
          s1 = fzero(s1) ? 0 : s1 + alpha_min;
          int alpha1 = int(distance_part1*s1);

          double dfm_mod2 = dfm*line_length_max/line_lengths[line_index_high];
          double part2 = pow(fbounds(1.1 - dfm_mod2/line_length_max, 0.0, 1.0), 2);
          double s2 = line_strengths[line_index_high]*part2*(alpha_max-alpha_min);
          s2 = fzero(s2) ? 0 : s2 + alpha_min;
          int alpha2 = int(distance_part2*s2);

          alpha = std::max(alpha1, alpha2);
          if (alpha > 255) alpha = 255;
          if (alpha < 0) alpha = 0;
        }
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_spark);
    delete[] imgpxdata;
  }
  static void generate_explosion()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    random_seed = 191; // NOTE: Setting random.
    const int mapwidth = 64, mapheight = 64;
    int map[mapwidth*mapheight];
    int max_range = 28;
    for (int x = 0; x < mapwidth; x++)
    {
      for (int y = 0; y < mapheight; y++)
      {
        map[x + y*mapwidth] = 0;
      }
    }

    for (int spot = 0; spot < 1100; spot++)
    {
      int x = int(63*(1.0*get_next_random() / M));
      int y = int(63*(1.0*get_next_random() / M));
      double r1 = sqrt(pow(x-32,2) + pow(y-32,2));
      double r2 = std::max(abs(x-32),abs(y-32));
      double r = (r1+r2)/2; // Rounded rectangle distance.
      int dist_add = int(4*(1-r/32))-2;
      int value = int(12 + 5*(1.0*get_next_random() / M)) + dist_add;
      if (max_range*0.95 <= r) continue;
      for (int dx = -3; dx < 4; dx++)
      {
        for (int dy = -3; dy < 4; dy++)
        {
          int fx = bounds(x-dx, 0, 63), fy = bounds(y-dy, 0, 63);
          map[fx + fy*fullwidth] += value;
        }
      }
    }
    for (int x = 0; x < mapwidth; x++)
    {
      for (int y = 0; y < mapheight; y++)
      {
        map[x + y*mapwidth] = std::min(map[x + y*mapwidth], 255);
      }
    }

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        // Interpolate from map.
        int sum = 0;
        int range = 3;
        int count = 0;
        for (int dx = -range; dx < range+1; dx++)
        {
          for (int dy = -range; dy < range+1; dy++)
          {
            if (sqrt(dx*dx+dy*dy) > range) continue;
            int fx = bounds(x-dx, 0, 63), fy = bounds(y-dy, 0, 63);
            sum += map[fx + fy*fullwidth];
            count++;
          }
        }
        count = count != 0 ? count : 1;
        int alpha = sum/count;
        if (alpha != 0) alpha = bounds(alpha + (get_next_random() % 14) - 7, 0, 255); // Add noise.

        int grayvalue = 255;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_explosion);
    delete[] imgpxdata;
  }
  static void generate_cloud()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    random_seed = 191; // NOTE: Setting random.
    const int mapwidth = 64, mapheight = 64;
    int map[mapwidth*mapheight];
    int max_range = 28;
    for (int x = 0; x < mapwidth; x++)
    {
      for (int y = 0; y < mapheight; y++)
      {
        map[x + y*mapwidth] = 0;
      }
    }

    for (int spot = 0; spot < 200; spot++)
    {
      int x = int(63*(1.0*get_next_random() / M));
      int y = int(63*(1.0*get_next_random() / M));
      double r1 = sqrt(pow(x-32,2) + pow(y-32,2));
      double r2 = std::max(abs(x-32),abs(y-32));
      double r = (r1+r2)/2; // Rounded rectangle distance.
      int dist_add = int(4*(1-r/32))-2;
      int value = int(12 + 5*(1.0*get_next_random() / M)) + dist_add;
      if (max_range*0.95 <= r) continue;
      for (int dx = -3; dx < 4; dx++)
      {
        for (int dy = -3; dy < 4; dy++)
        {
          int fx = bounds(x-dx, 0, 63), fy = bounds(y-dy, 0, 63);
          map[fx + fy*fullwidth] += value;
        }
      }
    }
    for (int x = 0; x < mapwidth; x++)
    {
      for (int y = 0; y < mapheight; y++)
      {
        map[x + y*mapwidth] = std::min(map[x + y*mapwidth], 255);
      }
    }

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        // Interpolate from map.
        int sum = 0;
        int range = 3;
        int count = 0;
        for (int dx = -range; dx < range+1; dx++)
        {
          for (int dy = -range; dy < range+1; dy++)
          {
            if (sqrt(dx*dx+dy*dy) > range) continue;
            int fx = bounds(x-dx, 0, 63), fy = bounds(y-dy, 0, 63);
            sum += map[fx + fy*fullwidth];
            count++;
          }
        }
        count = count != 0 ? count : 1;
        int alpha = sum/count;
        if (alpha != 0) alpha = bounds(alpha + (get_next_random() % 14) - 7, 0, 255); // Add noise.

        int grayvalue = 255;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_cloud);
    delete[] imgpxdata;
  }
  static void generate_smoke()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    random_seed = 191; // NOTE: Setting random.
    const int mapwidth = 64, mapheight = 64;
    int map[mapwidth*mapheight];
    int max_range = 28;
    for (int x = 0; x < mapwidth; x++)
    {
      for (int y = 0; y < mapheight; y++)
      {
        map[x + y*mapwidth] = 0;
      }
    }

    for (int spot = 0; spot < 1465; spot++)
    {
      int x = int(63*(1.0*get_next_random() / M));
      int y = int(63*(1.0*get_next_random() / M));
      double r1 = sqrt(pow(x-32,2) + pow(y-32,2));
      double r2 = std::max(abs(x-32),abs(y-32));
      double r = (r1+r2)/2; // Rounded rectangle distance.
      int value = int(3 + 3*(1.0*get_next_random() / M));
      if (max_range*0.7 <= r) continue;
      for (int dx = -10; dx < 11; dx++)
      {
        for (int dy = -10; dy < 11; dy++)
        {
          if (sqrt(dx*dx + dy*dy) > 10) continue;
          int fx = bounds(x-dx, 0, 63), fy = bounds(y-dy, 0, 63);
          map[fx + fy*fullwidth] += int(value * (1.0 - 1.0*map[fx + fy*fullwidth]/216));
        }
      }
    }
    for (int x = 0; x < mapwidth; x++)
    {
      for (int y = 0; y < mapheight; y++)
      {
        map[x + y*mapwidth] = bounds(map[x + y*mapwidth], 0, 255);
      }
    }

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        // Interpolate from map.
        int sum = 0;
        int range = 3;
        int count = 0;
        for (int dx = -range; dx < range+1; dx++)
        {
          for (int dy = -range; dy < range+1; dy++)
          {
            if (sqrt(dx*dx+dy*dy) > range) continue;
            int fx = bounds(x-dx, 0, 63), fy = bounds(y-dy, 0, 63);
            sum += map[fx + fy*fullwidth];
            count++;
          }
        }
        count = count != 0 ? count : 1;
        int alpha = sum/count;

        int grayvalue = 255;
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_smoke);
    delete[] imgpxdata;
  }
  static void generate_snow()
  {
    const int fullwidth = 64, fullheight = 64;
    unsigned char *imgpxdata = new unsigned char[4*fullwidth*fullheight+1];

    const int center_x = fullwidth/2-1, center_y = fullheight/2-1;

    const double radius = 24;
    V2 ss[6];
    for (int i = 0; i < 6; i++)
    {
      ss[i] = V2(center_x + radius*cos(i*M_PI*2/6 + M_PI/2), center_y - radius*sin(i*M_PI*2/6 + M_PI/2));
    }

    const int triangle_count = 8;
    V2 cs[triangle_count][3];
    cs[0][0] = ss[0], cs[0][1] = ss[2], cs[0][2] = ss[4];
    cs[1][0] = ss[1], cs[1][1] = ss[3], cs[1][2] = ss[5];

    for (int i = 0; i < 6; i++)
    {
      V2 vector1 = ss[(i+2) % 6].minus(ss[i]).multiply(1.0/3.0);
      V2 vector2 = ss[(i+1) % 6].minus(ss[(i+5) % 6]).multiply(1.0/3.0);
      // Koch snowflake, iteration 3, constructed with 8 triangles.
      V2 p1 = ss[i].add(vector1.multiply(1.0/3.0)).add(vector2.multiply(1.0/3.0));
      V2 p2 = ss[i].add(vector1.multiply(1.0/3.0)).minus(vector2.multiply(2.0/3.0));
      V2 p3 = p2.add(vector1);
      // GM snowflake approximation modification.
      /*V2 vector3 = ss[(i+3) % 6].minus(ss[i]);
      p1 = p1.minus(vector2.multiply(1.0/6.0)).minus(vector3.multiply(1.0/36.0));
      p2 = p2.add(vector2.multiply(1.0/6.0)).minus(vector3.multiply(1.0/36.0));
      p3 = p3.add(vector3.multiply(1.0/2.0));*/
      cs[i+2][0] = p1;
      cs[i+2][1] = p3;
      cs[i+2][2] = p2;
    }

    const double fade_dist = 4.0;

    for (int x = 0; x < fullwidth; x++)
    {
      for (int y = 0; y < fullheight; y++)
      {
        V2 p(x,y);
        int grayvalue = 255;
        int alpha = 0;
        bool inside = false;
        for (int i = 0; i < triangle_count; i++)
        {
          bool inside_triangle = true;
          for (int a = 0; a < 3; a++)
          {
            V2 triangle_vector = cs[i][(a+1) % 3].minus(cs[i][a]);
            V2 coord_to_point = p.minus(cs[i][a]);
            if (triangle_vector.crossproduct(coord_to_point) > 0) {
              inside_triangle = false;
              break;
            }
          }
          if (inside_triangle) {
            inside = true;
            break;
          }
        }
        if (inside) {
          alpha = 255;
        }
        else {
          alpha = 0;
          double min_dist = 1000.0;
          for (int i = 0; i < triangle_count; i++)
          {
            for (int a = 0; a < 3; a++)
            {
              // Find distance between point and triangle side.
              V2 triangle_vector = cs[i][(a+1) % 3].minus(cs[i][a]);
              V2 unit_vector = triangle_vector.unit_vector();
              V2 coord_to_point = p.minus(cs[i][a]);
              double dotproduct = unit_vector.dotproduct(coord_to_point);
              if (dotproduct <= 0 || dotproduct >= triangle_vector.norm()) { // If near the points.
                double xd1 = p.x - cs[i][a].x, yd1 = p.y - cs[i][a].y;
                double xd2 = p.x - cs[i][(a+1) % 3].x, yd2 = p.y - cs[i][(a+1) % 3].y;
                min_dist = std::min(min_dist, sqrt(xd1*xd1 + yd1*yd1));
                min_dist = std::min(min_dist, sqrt(xd2*xd2 + yd2*yd2));
              }
              else {
                V2 coordvector_on_trianglevector = unit_vector.multiply(dotproduct);
                V2 cross_vector = coordvector_on_trianglevector.minus(coord_to_point);
                double distance = cross_vector.norm();
                min_dist = std::min(min_dist, distance);
              }
            }
          }
          if (min_dist <= fade_dist) {
            alpha = bounds(int(255*(1.0 - min_dist/fade_dist)), 0, 255);
          }
        }
        imgpxdata[4*(x + y*fullwidth)+2] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+1] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+0] = grayvalue;
        imgpxdata[4*(x + y*fullwidth)+3] = alpha;
      }
    }
    create_particle_sprite(fullwidth, fullheight, fullwidth, fullheight, imgpxdata, pt_sh_snow);
    delete[] imgpxdata;
  }
  particle_sprite* get_particle_sprite(pt_shape particle_shape)
  {
    {
      std::map<pt_shape,enigma::particle_sprite*>::iterator it_shape = enigma::shape_to_sprite.find(particle_shape);
      if (it_shape != enigma::shape_to_sprite.end()) {
        return (*it_shape).second;
      }
    }
    // The particle shape may not have been generated yet.
    // If so, generate it and try again.
    switch(particle_shape) {
    case pt_sh_pixel: {generate_pixel(); break;}
    case pt_sh_disk: {generate_disk(); break;}
    case pt_sh_square: {generate_square(); break;}
    case pt_sh_line: {generate_line(); break;}
    case pt_sh_star: {generate_star(); break;}
    case pt_sh_circle: {generate_circle(); break;}
    case pt_sh_ring: {generate_ring(); break;}
    case pt_sh_sphere: {generate_sphere(); break;}
    case pt_sh_flare: {generate_flare(); break;}
    case pt_sh_spark: {generate_spark(); break;}
    case pt_sh_explosion: {generate_explosion(); break;}
    case pt_sh_cloud: {generate_cloud(); break;}
    case pt_sh_smoke: {generate_smoke(); break;}
    case pt_sh_snow: {generate_snow(); break;}
    default:
      #if DEBUG_MODE
        DEBUG_MESSAGE("No such particle type" + std::to_string(particle_shape), MESSAGE_TYPE::M_USER_ERROR)
      #endif
      ;
    }

    std::map<pt_shape,enigma::particle_sprite*>::iterator it_shape = enigma::shape_to_sprite.find(particle_shape);
    if (it_shape != enigma::shape_to_sprite.end()) {
      return (*it_shape).second;
    }
    return 0;
  }
  int get_particle_actual_sprite(pt_shape particle_shape)
  {
    {
      std::map<pt_shape,int>::iterator it_shape = enigma::shape_to_actual_sprite.find(particle_shape);
      if (it_shape != enigma::shape_to_actual_sprite.end()) {
        return (*it_shape).second;
      }
    }
    particle_sprite* ps = get_particle_sprite(particle_shape);
    if (ps == 0) {
      return -1;
    }
    
    enigma::Sprite spr(ps->width, ps->height, ps->width/2.0, ps->height/2.0);
    spr.SetBBox(0, 0, ps->height, ps->width);
    spr.AddSubimage(ps->texture, TexRect { 0, 0, 1, 1 }, ct_bbox);
    int sprid = enigma::sprites.add(std::move(spr));
    
    shape_to_actual_sprite.insert(std::pair<pt_shape,int>(particle_shape,sprid));
    return sprid;
  }
}
