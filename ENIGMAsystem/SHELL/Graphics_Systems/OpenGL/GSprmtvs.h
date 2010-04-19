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

const int pr_pointlist      =1;     //GL_POINTS
const int pr_linelist       =2;     //GL_LINES
const int pr_linestrip      =3;     //GL_LINE_STRIP
const int pr_trianglelist   =4;     //GL_TRIANGLES
const int pr_trianglestrip  =5;     //GL_TRIANGLE_STRIP
const int pr_trianglefan    =6;     //GL_TRIANGLE_FAN

const int pr_lineloop       =7;     //GL_LINE_LOOP
const int pr_quadlist       =8;     //GL_QUADS
const int pr_quadstrip      =9;     //GL_QUAD_STRIP
const int pr_polygon       =10;     //GL_POLYGON

int draw_primitive_begin(double kind);
int draw_vertex(double x, double y);
int draw_vertex_color(float x, float y, int color, float alpha);
int draw_primitive_end();
