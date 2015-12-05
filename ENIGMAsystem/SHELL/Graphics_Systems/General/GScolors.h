/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#ifndef ENIGMA_GLCOLORS_H
#define ENIGMA_GLCOLORS_H

namespace enigma_user
{

enum {
  c_aqua    = 0xFFFF00,
  c_black   = 0x000000,
  c_blue    = 0xFF0000,
  c_dkgray  = 0x404040,
  c_fuchsia = 0xFF00FF,
  c_gray    = 0x808080,
  c_green   = 0x008000,
  c_lime    = 0x00FF00,
  c_ltgray  = 0xC0C0C0,
  c_maroon  = 0x000080,
  c_navy    = 0x800000,
  c_olive   = 0x008080,
  c_orange  = 0x40A0FF,
  c_purple  = 0x800080,
  c_red     = 0x0000FF,
  c_silver  = 0xC0C0C0,
  c_teal    = 0x808000,
  c_white   = 0xFFFFFF,
  c_yellow  = 0x00FFFF
};

void draw_clear(int color);
void draw_clear_alpha(int color, float alpha);

int merge_color(int col1, int col2, double amount);

void draw_set_color(int col);
void draw_set_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
void draw_set_alpha(float alpha);
void draw_set_color_rgba(unsigned char red, unsigned char green, unsigned char blue, float alpha);
void draw_set_color_write_enable(bool red, bool green, bool blue, bool alpha);
int draw_get_color();
int draw_get_red();
int draw_get_green();
int draw_get_blue();
float draw_get_alpha();

int make_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
int make_color_rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
inline int make_color(unsigned char r, unsigned char g, unsigned char b)
{
    return make_color_rgb(r,g,b);
}

int color_get_red(int color);
int color_get_green(int color);
int color_get_blue(int color);

int color_get_hue(int color);
int color_get_value(int color);
int color_get_saturation(int color);

int make_color_hsv(int hue,int saturation,int value);

//UK localisation
//This has been done because GameMaker:Studio allows for both spellings of 'color'
inline int merge_colour(int col1, int col2, double amount)
{
  return merge_color(col1, col2, amount);
}
inline void draw_set_colour(int col)
{
  draw_set_color(col);
}
inline void draw_set_colour_rgb(unsigned char red, unsigned char green, unsigned char blue)
{
  draw_set_color_rgb(red, green, blue);
}
inline void draw_set_colour_rgba(unsigned char red, unsigned char green, unsigned char blue, float alpha)
{
  draw_set_color_rgba(red, green, blue, alpha);
}
inline void draw_set_colour_write_enable(bool red, bool green, bool blue, bool alpha)
{
  draw_set_color_write_enable(red, green, blue, alpha);
}
inline int draw_get_colour()
{
  return draw_get_color();
}
inline int make_colour_rgb(unsigned char red, unsigned char green, unsigned char blue)
{
  return make_color_rgb(red, green, blue);
}
inline int make_colour_rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
  return make_color_rgba(red, green, blue, alpha);
}
inline int make_colour(unsigned char r, unsigned char g, unsigned char b)
{
    return make_color_rgb(r,g,b);
}
inline int colour_get_red(int color)
{
  return color_get_red(color);
}
inline int colour_get_green(int color)
{
  return color_get_green(color);
}
inline int colour_get_blue(int color)
{
  return color_get_blue(color);
}
inline int colour_get_hue(int color)
{
  return color_get_hue(color);
}
inline int colour_get_value(int color)
{
  return color_get_value(color);
}
inline int colour_get_saturation(int color)
{
  return color_get_saturation(color);
}
inline int make_colour_hsv(int hue,int saturation,int value)
{
  return make_color_hsv(hue,saturation,value);
}

}

#endif // ENIGMA_GLCOLORS_H
