/**
 * @file svg_simple.cpp
 * @brief Source implementing SVG functions.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "svg_simple.h"

using std::string;
std::string SVG::escape(std::string e) {
  string r = e;
  for (size_t i = 0; i < r.length(); i++)
    if (r[i] == '<')
      r.replace(i,1,"&lt;");
    else if (r[i] == '>')
      r.replace(i,1,"&gt;");
    else if (r[i] == '&')
      r.replace(i,1,"&amp;");
  return r;
}
std::string SVG::tostring(int id) { char buf[16]; sprintf(buf,"%d",id); return buf; }

void SVG::write_header(int w, int h) {
  fputs("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>", f);
  fprintf(f,
    "<svg xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:cc=\"http://creativecommons.org/ns#\" xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" "
    "xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\" width=\"%d\" "
    "height=\"%d\" id=\"JDI_AST_Render\" version=\"1.1\" sodipodi:docname=\"JDI AST Render\">\n",w,h);
  fputs("<g id=\"Drawing\">\n", f);
}
void SVG::draw_circle(std::string id, int x,int y,int r,unsigned fill,unsigned stroke, double stroke_width) {
  const int d = r * 2;
  fprintf(f, "  <path sodipodi:type=\"arc\" sodipodi:cx=\"%d\" sodipodi:cy=\"%d\" sodipodi:rx=\"%d\" sodipodi:ry=\"%d\" "
             "style=\"fill:#%06X;fill-opacity:%f;stroke:#%06X;stroke-opacity:%f;stroke-width:%f;\" id=\"%s\" "
             "d=\"m %d,%d a %d,%d 0 0 1 -%d,0 %d,%d 0 0 1 %d,0 z\" />\n", x,y,r,r,
          fill&0x00FFFFFF, (fill&0xFF000000)/(double)0xFF000000, stroke&0x00FFFFFF,(stroke&0xFF000000)/(double)0xFF000000, stroke_width, id.c_str(), x+r,y, r,r, d, r,r, d);
}
void SVG::draw_line(std::string id, int x1, int y1, int x2, int y2, unsigned stroke, float stroke_width) {
  fprintf(f, "  <path style=\"fill:none;stroke:#%06X;stroke-width:%f;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:%f;stroke-miterlimit:4;stroke-dasharray:none\" "
             "d=\"M %d,%d %d,%d\" id=\"%s\" />\n", stroke & 0x00FFFFFF, stroke_width, (stroke & 0xFF000000) / (double)0xFF000000, x1,y1, x2,y2, id.c_str());
}
void SVG::draw_rectangle(std::string id, int x1, int y1, int x2, int y2, unsigned fill, unsigned stroke, float stroke_width) {
  fprintf(f, "  <path style=\"fill:#%06X;fill-opacity:%f;stroke:#%06X;stroke-width:%f;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:%f;stroke-miterlimit:4;stroke-dasharray:none\" "
             "d=\"M %d,%d %d,%d %d,%d %d,%d %d,%d\" id=\"%s\" />\n", fill & 0x00FFFFFF, (fill & 0xFF000000) / (double)0xFF000000, stroke & 0x00FFFFFF, stroke_width, (stroke & 0xFF000000) / (double)0xFF000000, x1,y1, x2,y1, x2,y2, x1,y2, x1,y1, id.c_str());
}
void SVG::draw_text(std::string id, int cx, int bly, string t, int sz, unsigned color) {
  fprintf(f,
  "  <text xml:space=\"preserve\" style=\"font-size:%dpx;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;"
  "line-height:125%%;letter-spacing:0px;word-spacing:0px;fill:#%06X;fill-opacity:%f;stroke:none;font-family:Monospace;\" x=\"%d\" y=\"%d\" id=\"%s\" "
  "sodipodi:linespacing=\"125%%\"><tspan\n    sodipodi:role=\"line\" id=\"%s_span\" x=\"%d\" y=\"%d\" style=\"text-align:center;text-anchor:middle;\">%s</tspan></text>\n",
  sz, color & 0x00FFFFFF, (color & 0xFF000000) / (double)0xFF000000, cx, bly, id.c_str(), id.c_str(), cx, bly, escape(t).c_str());
}
bool SVG::is_open() {
  return f;
}
void SVG::close() {
  fputs("</g>\n",f);
  fputs("</svg>\n",f);
  fclose(f);
  f = NULL;
}
SVG::SVG(const char* fn) {
  f = fopen(fn, "wb");
}
SVG::~SVG() {
  if (f) close();
}
