/** Copyright (C) 2008-2013 Josh Ventura
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

void draw_set_font(int fnt);
void draw_text(int x,int y,string str);
void draw_text_color(int x,int y,string str,int c1,int c2,int c3,int c4,double a);

void draw_text_ext(int x,int y,string str, int sep, int w);
void draw_text_ext_color(int x,int y,string str, int sep,int w,int c1,int c2,int c3,int c4,double a);

void draw_text_transformed(double x,double y,string str,double xscale,double yscale,double rot);
void draw_text_transformed_color(double x,double y,string str,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a);

void draw_text_ext_transformed(double x,double y,string str,int sep,int w, double xscale,double yscale,double rot);
void draw_text_ext_transformed_color(double x,double y,string str,int sep,int w,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a);

unsigned int font_get_texture(int fnt);
int  font_add_sprite(int spr, unsigned char first, bool prop, int sep);
unsigned int font_get_texture_width(int fnt);
unsigned int font_get_texture_height(int fnt);

unsigned int string_width(string str);
unsigned int string_height(string str);

unsigned int string_width_ext(string str, int sep, int w);
unsigned int string_height_ext(string str, int sep, int w);

unsigned int string_width_line(string str, int line);
unsigned int string_width_ext_line(string str, int w, int line);
unsigned int string_width_ext_line_count(string str, int w);
