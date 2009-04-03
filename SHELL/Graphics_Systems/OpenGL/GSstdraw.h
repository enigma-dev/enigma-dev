/*********************************************************************************\
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
\*********************************************************************************/

/**Standard drawing functions**************************************************\

int draw_point(double x, double y)
int draw_point_color(double x, double y,double color)
int draw_line(double x1,double y1,double x2,double y2)
int draw_line_width(double x1,double y1,double x2,double y2,double width)
int draw_line_color(double x1,double y1,double x2,double y2,double color1,double color2)
int draw_line_width_color(double x1,double y1,double x2,double y2,double width,double color1,double color2)
int draw_rectangle(double x1,double y1,double x2,double y2,double outline)
int draw_rectangle_angle(double x1,double y1,double x2,double y2,int outline,double angle)
int draw_rectangle_color(double x1,double y1,double x2,double y2,double color1,double color2,double color3,double color4,int outline)
int draw_set_circle_precision(int precision)
int draw_circle(double x, double y, double radius, double outline)
int draw_circle_color(double x, double y, double radius, double color1, double color2, double outline)
int draw_circle_smooth(double x, double y, double radius, int outline)
int draw_circle_smooth_color(double x, double y, double radius, double outline, double color1, double color2)
int draw_ellipse(double x1,double y1,double x2,double y2,double outline)
int draw_triangle(double x1,double y1,double x2,double y2,double x3,double y3,double outline)
int draw_triangle_color(double x1,double y1,double x2,double y2,double x3,double y3,double col1,double col2,double col3,double outline)
int draw_roundrect(double x1,double y1,double x2,double y2,double outline)

\******************************************************************************/


int draw_point(double x, double y);
int draw_point_color(double x, double y,double color);
int draw_line(double x1,double y1,double x2,double y2);
int draw_line_width(double x1,double y1,double x2,double y2,double width);
int draw_line_color(double x1,double y1,double x2,double y2,double color1,double color2);
int draw_line_width_color(double x1,double y1,double x2,double y2,double width,double color1,double color2);
int draw_rectangle(double x1,double y1,double x2,double y2,double outline);
int draw_rectangle_angle(double x1,double y1,double x2,double y2,int outline,double angle);
int draw_rectangle_color(double x1,double y1,double x2,double y2,double color1,double color2,double color3,double color4,int outline);
int draw_set_circle_precision(int precision);
int draw_circle(double x, double y, double radius, double outline);
int draw_circle_color(double x, double y, double radius, double color1, double color2, double outline);
int draw_circle_smooth(double x, double y, double radius, int outline);
int draw_circle_smooth_color(double x, double y, double radius, double outline, double color1, double color2);
int draw_ellipse(double x1,double y1,double x2,double y2,double outline);
int draw_triangle(double x1,double y1,double x2,double y2,double x3,double y3,double outline);
int draw_triangle_color(double x1,double y1,double x2,double y2,double x3,double y3,double col1,double col2,double col3,double outline);
int draw_roundrect(double x1,double y1,double x2,double y2,double outline);
