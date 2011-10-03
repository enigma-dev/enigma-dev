/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura, Harijs Grînbergs                           **
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
const unsigned fa_left = 0;
const unsigned fa_center = 1;
const unsigned fa_right = 2;
const unsigned fa_top = 0;
const unsigned fa_middle = 1;
const unsigned fa_bottom = 2;

void draw_set_halign(unsigned align);
unsigned draw_get_halign();
void draw_set_valign(unsigned align);
unsigned draw_get_valign();
void draw_set_font(int fnt);
int draw_get_font();
void draw_text(int x,int y,variant str);
void draw_text_color(int x,int y,variant str,int c1,int c2,int c3,int c4,double a);

void draw_text_ext(int x,int y,variant str, int sep, int w);
void draw_text_ext_color(int x,int y,variant str, int sep,int w,int c1,int c2,int c3,int c4,double a);

void draw_text_transformed(double x,double y,variant str,double xscale,double yscale,double rot);
void draw_text_transformed_color(double x,double y,variant str,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a);

void draw_text_ext_transformed(double x,double y,variant str,int sep,int w, double xscale,double yscale,double rot);
void draw_text_ext_transformed_color(double x,double y,variant str,int sep,int w,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a);

unsigned int font_get_texture(int fnt);
unsigned int font_get_texture_width(int fnt);
unsigned int font_get_texture_height(int fnt);

unsigned int string_width(variant str);
unsigned int string_height(variant str);

unsigned int string_width_ext(variant str, int sep, int w);
unsigned int string_height_ext(variant str, int sep, int w);

unsigned int string_width_line(variant str, int line);
unsigned int string_width_ext_line(variant str, int w, int line);
unsigned int string_width_ext_line_count(variant str, int w);
