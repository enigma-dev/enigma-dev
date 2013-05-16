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

/** Clears everything drawn to the given color.
@param color
**/
void draw_clear(int color);
/** Clears everything drawn to the given color with the given alpha.
@param color the color to clear with
@param alpha the alpha to clear with
**/
void draw_clear_alpha(int color, float alpha);

/** Merges two colors into a single color value.
@param col1 the first color
@param col2 the second color
@param amount fractional amount of the second color
@return This function returns two colors merged into a single value.
**/
int merge_color(int col1, int col2, double amount);

/** Sets the color used for drawing.
@param col the color to draw with
**/
void draw_set_color(int col);
/** Sets the color used for drawing.
@param red the red component of the color
@param green the green component of the color
@param blue the blue component of the color
**/
void draw_set_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
/** Sets the alpha used for drawing.
@param alpha the alpha value to draw with
**/
void draw_set_alpha(float alpha);
/** Sets the color and alpha value to draw with.
@param red the red component of the color
@param green the green component of the color
@param blue the blue component of the color
@param alpha the alpha component of the color
**/
void draw_set_color_rgba(unsigned char red, unsigned char green, unsigned char blue, float alpha);
/** Gets the current color used for drawing.
@return This function returns the current draw color.
**/
int draw_get_color();
/** Gets the red component of the current color used for drawing.
@return This function returns the red component of the current draw color.
**/
int draw_get_red();
/** Gets the green component of the current color used for drawing.
@return This function returns the green component of the current draw color.
**/
int draw_get_green();
/** Gets the blue component of the current color used for drawing.
@return This function returns the blue component of the current draw color.
**/
int draw_get_blue();
/** Gets the current alpha used for drawing.
@return This function returns the current drawing alpha.
**/
float draw_get_alpha();

/** description
@param red
@param green
@param blue
@return
**/
int make_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
/** description
@param r
@param g
@param b
@return
**/
inline int make_color(unsigned char r, unsigned char g, unsigned char b)
{
    return make_color_rgb(r,g,b);
}

/** description
@param color
@return 
**/
int color_get_red(int color);
/** description
@param color
@return 
**/
int color_get_green(int color);
/** description
@param color
@return 
**/
int color_get_blue(int color);

/** description
@param color
@return 
**/
int color_get_hue(int color);
/** description
@param color
@return 
**/
int color_get_value(int color);
/** description
@param color
@return 
**/
int color_get_saturation(int color);

/** description
@param hue
@param saturation
@param value
@return 
**/
int make_color_hsv(int hue,int saturation,int value);

/** description
**/
void draw_unbind_all();

}

