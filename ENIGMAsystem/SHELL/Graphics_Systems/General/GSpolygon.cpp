/** Copyright (C) 2013 Robert B. Colton, 2013-2015 Harijs Grinbergs
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

#include "GScolors.h"
#include "GScolor_macros.h"
#include "GSsprite.h"
#include "GStextures.h"
#include "GSprimitives.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/math_consts.h"
#include "Universal_System/Resources/polygon.h"
#include "Universal_System/Resources/polygon_internal.h"

#include <cmath>
#include <cstdlib>
#include <string>

namespace enigma_user {
  void draw_polygon(int polygon_id, gs_scalar x, gs_scalar y, double angle, gs_scalar xscale, gs_scalar yscale) 
  {
    // Retrieving points of the polygon
    enigma::Polygon poly = enigma::polygons.get(polygon_id);
    std::vector<glm::vec2> points = poly.getPoints();
    glm::vec2 pivot = poly.getOffset();
    enigma::transformPoints(points, x, y, angle, pivot, xscale, yscale);
    int N = points.size();

    // Drawing from point i to i + 1
    for (int i = 0; i < N - 1; ++i) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex(points[i].x, points[i].y);
      draw_vertex(points[i + 1].x, points[i + 1].y);
      draw_primitive_end();
    }

    // Drawing the last line to complete the polygon
    draw_primitive_begin(pr_linestrip);
    draw_vertex(points[N - 1].x, points[N - 1].y);
    draw_vertex(points[0].x, points[0].y);
    draw_primitive_end();
  }

  void draw_polygon_bbox(int polygon_id, gs_scalar x, gs_scalar y, double angle, gs_scalar xscale, gs_scalar yscale) 
  {
    // Computing BBOX
    enigma::Polygon poly = enigma::polygons.get(polygon_id);
    std::vector<glm::vec2> points = poly.getPoints();
    glm::vec2 pivot = poly.getOffset();
    enigma::transformPoints(points, x, y, angle, pivot, xscale, yscale);
    enigma::Rect<int> bbox = enigma::computeBBOXFromPoints(points);

    // Drawing
    draw_primitive_begin(pr_linestrip);
    draw_vertex(bbox.left(), bbox.top());
    draw_vertex(bbox.right(), bbox.top());
    draw_vertex(bbox.right(), bbox.bottom());
    draw_vertex(bbox.left(), bbox.bottom());
    draw_vertex(bbox.left(), bbox.top());
    draw_primitive_end();
  }

  void draw_polygon_sub(int polygon_id, gs_scalar x, gs_scalar y, double angle, gs_scalar xscale, gs_scalar yscale)
  {
    // Retrieving points of the polygon
    enigma::Polygon poly = enigma::polygons.get(polygon_id);
    std::vector<glm::vec2> points = poly.getPoints();
    glm::vec2 pivot = poly.getOffset();
    enigma::transformPoints(points, x, y, angle, pivot, xscale, yscale);
    int N = points.size();

    // Drawing from point i to i + 1
    for (int i = 0; i < N - 1; ++i) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex(points[i].x, points[i].y);
      draw_vertex(points[i + 1].x, points[i + 1].y);
      draw_primitive_end();
    }

    // Drawing the last line to complete the polygon
    draw_primitive_begin(pr_linestrip);
    draw_vertex(points[N - 1].x, points[N - 1].y);
    draw_vertex(points[0].x, points[0].y);
    draw_primitive_end();

    // Drawing diagonals
    std::vector<enigma::Diagonal> diagonals = enigma::polygons.get(polygon_id).getDiagonals();
    std::vector<enigma::Diagonal>::iterator it;
    for (it = diagonals.begin(); it != diagonals.end(); ++it) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex(points[it->i].x, points[it->i].y);
      draw_vertex(points[it->j].x, points[it->j].y);
      draw_primitive_end();
    }
  }

  void draw_polygon_color(int polygon_id, gs_scalar x, gs_scalar y, double angle, gs_scalar xscale, gs_scalar yscale, int color)
  {
    // Retrieving points of the polygon
    enigma::Polygon poly = enigma::polygons.get(polygon_id);
    std::vector<glm::vec2> points = poly.getPoints();
    glm::vec2 pivot = poly.getOffset();
    enigma::transformPoints(points, x, y, angle, pivot, xscale, yscale);
    int N = points.size();

    // Getting the alpha of the drawing
    gs_scalar alpha = draw_get_alpha();

    // Drawing from point i to i + 1
    for (int i = 0; i < N - 1; ++i) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex_color(points[i].x, points[i].y, color, alpha);
      draw_vertex_color(points[i + 1].x, points[i + 1].y, color, alpha);
      draw_primitive_end();
    }

    // Drawing the last line to complete the polygon
    draw_primitive_begin(pr_linestrip);
    draw_vertex_color(points[N - 1].x, points[N - 1].y, color, alpha);
    draw_vertex_color(points[0].x, points[0].y, color, alpha);
    draw_primitive_end();
  }

  void draw_polygon_bbox_color(int polygon_id, gs_scalar x, gs_scalar y, double angle, gs_scalar xscale, gs_scalar yscale, int color)
  {
    // Computing BBOX
    enigma::Polygon poly = enigma::polygons.get(polygon_id);
    std::vector<glm::vec2> points = poly.getPoints();
    glm::vec2 pivot = poly.getOffset();
    enigma::transformPoints(points, x, y, angle, pivot, xscale, yscale);
    enigma::Rect<int> bbox = enigma::computeBBOXFromPoints(points); 

    // Setting Alpha
    gs_scalar alpha = draw_get_alpha();

    // Drawing
    draw_primitive_begin(pr_linestrip);
    draw_vertex_color(bbox.left(), bbox.top(), color, alpha);
    draw_vertex_color(bbox.right(), bbox.top(), color, alpha);
    draw_vertex_color(bbox.right(), bbox.bottom(), color, alpha);
    draw_vertex_color(bbox.left(), bbox.bottom(), color, alpha);
    draw_vertex_color(bbox.left(), bbox.top(), color, alpha);
    draw_primitive_end();
  }

  void draw_polygon_sub_color(int polygon_id, gs_scalar x, gs_scalar y, double angle, gs_scalar xscale, gs_scalar yscale, int c1, int c2)
  {
    // Retrieving points of the polygon
    enigma::Polygon poly = enigma::polygons.get(polygon_id);
    std::vector<glm::vec2> points = poly.getPoints();
    glm::vec2 pivot = poly.getOffset();
    enigma::transformPoints(points, x, y, angle, pivot, xscale, yscale);
    int N = points.size();

    // Getting the alpha of the drawing
    gs_scalar alpha = draw_get_alpha();

    // Drawing from point i to i + 1
    for (int i = 0; i < N - 1; ++i) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex_color(points[i].x, points[i].y, c1, alpha);
      draw_vertex_color(points[i + 1].x, points[i + 1].y, c1, alpha);
      draw_primitive_end();
    }

    // Drawing the last line to complete the polygon
    draw_primitive_begin(pr_linestrip);
    draw_vertex_color(points[N - 1].x, points[N - 1].y, c1, alpha);
    draw_vertex_color(points[0].x, points[0].y, c1, alpha);
    draw_primitive_end();

    // Drawing diagonals
    std::vector<enigma::Diagonal> diagonals = enigma::polygons.get(polygon_id).getDiagonals();
    std::vector<enigma::Diagonal>::iterator it;
    for (it = diagonals.begin(); it != diagonals.end(); ++it) {
      draw_primitive_begin(pr_linestrip);
      draw_vertex_color(points[it->i].x, points[it->i].y, c2, alpha);
      draw_vertex_color(points[it->j].x, points[it->j].y, c2, alpha);
      draw_primitive_end();
    }
  }
}
