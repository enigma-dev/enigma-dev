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

/**Some color functions********************************************************\

c_aqua, c_black, c_blue, c_dkgray, c_fuchsia, c_gray, c_green, c_lime, c_ltgray,
c_maroon, c_navy, c_olive, c_purple, c_red, c_silver ,c_teal, c_white, c_yellow

int draw_clear(double color)
int draw_clear_alpha(double color, double alpha)
int draw_set_color(double col)
int draw_set_color_rgb(double red, double green, double blue)
int draw_set_alpha(double alpha)
int draw_set_color_rgba(double red, double green, double blue, double alpha)
int draw_get_color()
int make_color(double red, double green, double blue)
int make_color_rgb(double red, double green, double blue)
int color_get_red(double color)
int color_get_green(double color)
int color_get_blue(double color)
double color_get_hue(double color)
double color_get_value(double color)
double color_get_saturation(double color)
int make_color_hsv(double hue,double saturation,double value)

\******************************************************************************/

const int c_aqua   =0xFFFF00;
const int c_black  =0x000000;
const int c_blue   =0xFF0000;
const int c_dkgray =0x404040;
const int c_fuchsia=0xFF00FF;
const int c_gray   =0x808080;
const int c_green  =0x008000;
const int c_lime   =0x00FF00;
const int c_ltgray =0xC0C0C0;
const int c_maroon =0x000080;
const int c_navy   =0x800000;
const int c_olive  =0x008080;
const int c_purple =0x800080;
const int c_red    =0x0000FF;
const int c_silver =0xC0C0C0;
const int c_teal   =0x808000;
const int c_white  =0xFFFFFF;
const int c_yellow =0x00FFFF;

int draw_clear(double color);
int draw_clear_alpha(double color, double alpha);

int merge_color(int col1, int col2, double amount);

int draw_set_color(double col);
int draw_set_color_rgb(double red, double green, double blue);
int draw_set_alpha(float alpha);
int draw_set_color_rgba(double red, double green, double blue, double alpha);
double draw_get_color();
float draw_get_red(void);
float draw_get_green(void);
float draw_get_blue(void);
float draw_get_alpha(void);


double make_color_rgb(double red, double green, double blue);
double(*const make_color)(double,double,double) = make_color_rgb;

double color_get_red(double color);
double color_get_green(double color);
double color_get_blue(double color);

double color_get_hue(double color);
double color_get_value(double color);
double color_get_saturation(double color);

double make_color_hsv(double hue,double saturation,double value);
