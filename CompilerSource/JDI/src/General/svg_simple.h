/**
 * @file svg_simple.h
 * @brief A small library for creating simple SVG images.
 *
 * @section License
 *
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 *
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 *
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/


#include <cstdio>
#include <string>
#include <string>
#include <filesystem>

/**
  A class for exporting simple SVG images. The goal is to get along with Firefox, Chrome, and ImageMagick without bloating
  the file down with InkScape extensions. As such, InkScape will correctly render exported files, but will not love them.
**/
class SVG {
  FILE* f;
 public:
  /** Escape a string for writing to the file outside of a tag.
      @param e  The string to be escaped.
      @return   Returns an escaped copy of the string. **/
  static std::string escape(std::string e);

  /** Print the basic SVG information. This should always be the first call.
      @param w  The width of the graphic, in pixels.
      @param h  The height of the graphic, in pixels. **/
  void write_header(int w, int h);

  /** Render a circle to this SVG.
      @param id            The unique id of this circle, as a string.
      @param x             The x coordinate of the center of the circle.
      @param y             The y coordinate of the center of the circle.
      @param r             The radius of the circle.
      @param fill          The fill color, in ARGB format. Defaults to full-alpha white.
      @param stroke        The stroke color, in ARGB format. Defaults to full-alpha black.
      @param stroke_width  The width of the stroke, in pixels. Defaults to 2. **/
  void draw_circle(std::string id, int x,int y,int r,unsigned fill = 0xFFFFFFFF,
                   unsigned stroke = 0xFF000000, double stroke_width = 2);

  /** Render a line to this SVG.
      @param id            The unique id of this line, as a string.
      @param x1            The x coordinate of the first point on the line.
      @param y1            The y coordinate of the first point on the line.
      @param x2            The x coordinate of the second point on the line.
      @param y2            The y coordinate of the second point on the line.
      @param stroke        The stroke color, in ARGB format. Defaults to full-alpha black.
      @param stroke_width  The width of the stroke, in pixels. Defaults to 2. **/
  void draw_line(std::string id, int x1, int y1, int x2, int y2,
                 unsigned stroke = 0xFF000000, float stroke_width = 2);

  /** Render a rectangle to this SVG.
      @param id            The unique id of this rectangle, as a string.
      @param x1            The x coordinate of the top-left corner of the rectangle.
      @param y1            The y coordinate of the top-left corner of the rectangle.
      @param x2            The x coordinate of the bottom-right corner of the rectangle.
      @param y2            The y coordinate of the bottom-right corner of the rectangle.
      @param fill          The fill color, in ARGB format. Defaults to full-alpha white.
      @param stroke        The stroke color, in ARGB format. Defaults to full-alpha white.
      @param stroke_width  The width of the stroke, in pixels. Defaults to 2. **/
  void draw_rectangle(std::string id, int x1, int y1, int x2, int y2,
                      unsigned fill, unsigned stroke = 0xFF000000, float stroke_width = 2);

  /** Render text to this SVG.
      @param id            The unique id of the text, as a string.
      @param cx            The x coordinate of the center of the text's baseline.
      @param bly           The y coordinate of the text's baseline.
      @param text          The text to be drawn.
      @param sz            The size of the font.
      @param color         The text color, in ARGB format. Defaults to full-alpha black. **/
  void draw_text(std::string id, int cx, int bly, std::string text,
                 int sz = 12, unsigned color = 0xFF000000);

  /// Close the SVG file, writing closing body tags.
  void close();

  /// Check if the SVG file is currently open.
  /// @return Returns true (1) if the file is open, false (0) otherwise.
  bool is_open();

  /// Default constructor. Opens the file, but does not write header data.
  SVG(std::filesystem::path fn);

  /// Default destructor. Closes the file, writing closing tags, if it is still open.
  ~SVG();
};
