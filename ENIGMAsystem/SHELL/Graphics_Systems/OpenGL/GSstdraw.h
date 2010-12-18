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

void draw_set_line_pattern(unsigned short pattern, int scale);
int draw_point(float x, float y);
int draw_point_color(float x, float y,int color);
int draw_line(float x1,float y1,float x2,float y2);
int draw_line_width(float x1,float y1,float x2,float y2,float width);
int draw_line_color(float x1,float y1,float x2,float y2,int color1,int color2);
int draw_line_width_color(float x1,float y1,float x2,float y2,float width,int color1,int color2);
int draw_rectangle(float x1,float y1,float x2,float y2,bool outline);
int draw_rectangle_angle(float x1,float y1,float x2,float y2,float angle,bool outline);
int draw_rectangle_color(float x1,float y1,float x2,float y2,int color1,int color2,int color3,int color4,bool outline);
int draw_set_circle_precision(float precision);
float draw_get_circle_precision(void);
int draw_circle(float x, float y, float radius, bool outline);
int draw_circle_color(float x, float y, float radius, int color1, int color2, bool outline);
int draw_circle_perfect(float x, float y, float radius, bool outline);
int draw_circle_color_perfect(float x, float y, float radius, int color1, int color2, bool outline);
int draw_ellipse(float x1,float y1,float x2,float y2,bool outline);
int draw_ellipse_color(float x1, float y1, float x2, float y2, int col1, int col2, bool outline);
int draw_ellipse_perfect(float x1,float y1,float x2,float y2,bool outline);
int draw_triangle(float x1,float y1,float x2,float y2,float x3,float y3,bool outline);
int draw_triangle_color(float x1,float y1,float x2,float y2,float x3,float y3,int col1,int col2,int col3,bool outline);
int draw_roundrect(float x1,float y1,float x2,float y2, float r, bool outline);
int draw_roundrect_color(float x1, float y1, float x2, float y2, float r, int col1, int col2, bool outline);
int draw_healthbar(float x1, float y1, float x2, float y2, float amount, int backcol, int mincol, int maxcol, int direction, bool showback, bool showborder);
int draw_getpixel(int,int);
int(*const draw_get_pixel)(int,int)=draw_getpixel;
int draw_mandelbrot(float x, float y, float w, double Zx, double Zy, double Zw, unsigned iter);
int draw_arrow(float x1, float y1, float x2, float y2, float arrow_size, float line_size, bool outline);
void draw_button(float x1, float y1, float x2, float y2, float border_width, bool up);
