/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
  c_orange  = 0x0080FF,
  c_purple  = 0x800080,
  c_red     = 0x0000FF,
  c_silver  = 0xC0C0C0,
  c_teal    = 0x808000,
  c_white   = 0xFFFFFF,
  c_yellow  = 0x00FFFF
};

int draw_clear(int color);
int draw_clear_alpha(int color, float alpha);

int merge_color(int col1, int col2, double amount);

int draw_set_color(int col);
int draw_set_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
int draw_set_alpha(float alpha);
int draw_set_color_rgba(unsigned char red, unsigned char green, unsigned char blue, float alpha);
double draw_get_color();
float draw_get_red(void);
float draw_get_green(void);
float draw_get_blue(void);
float draw_get_alpha(void);


int make_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
int (*const make_color)(unsigned char,unsigned char,unsigned char) = make_color_rgb;

int color_get_red(int color);
int color_get_green(int color);
int color_get_blue(int color);

int color_get_hue(int color);
int color_get_value(int color);
int color_get_saturation(int color);

int make_color_hsv(int hue,int saturation,int value);
