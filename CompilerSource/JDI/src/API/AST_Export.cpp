/**
 * @file AST_Export.cpp
 * @brief Source implementing export functions for printing and rendering ASTs.
 *
 * @section License
 *
 * Copyright (C) 2011-2012 Josh Ventura
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

#include "AST.h"
#include <General/svg_simple.h>

#include <filesystem>

using std::max;

namespace jdi {

string AST::toString() const {
  if (root) return root->toString();
  return "";
}

/// A wrapper to \c SVG which helps generate IDs for each node.
/// The caller is meant to increment the internal node count after all entities
/// for that node have been written to the SVG.
struct SVGrenderInfo {
  SVG *svg;
  AST_Node *cur;
  size_t nodes_written = 0;

  void draw_circle(int nid, int x, int y, int r, unsigned fill,
                   unsigned stroke = 0xFF000000, int stroke_width = 2) {
    svg->draw_circle("Node"+std::to_string(nid),
                     x, y, r, fill, stroke, stroke_width);
  }
  void draw_rectangle(int nid, int x1, int y1, int x2, int y2, unsigned fill,
                      unsigned stroke = 0xFF000000,int stroke_width = 2) {
    svg->draw_rectangle("Node" + std::to_string(nid),
                        x1, y1, x2, y2, fill, stroke, stroke_width);
  }
  void draw_line(int nid,char s_id, int x1, int y1, int x2, int y2,
                 unsigned stroke = 0xFF000000, int stroke_width = 2) {
    svg->draw_line("Connector_" + std::to_string(nid) + "_" + s_id,
                   x1, y1, x2, y2, stroke, stroke_width);
  }
  void draw_text(int nid, int cx, int bly, string text,
                 unsigned fill = 0xFF000000) {
    svg->draw_text("Label_" + std::to_string(nid), cx, bly, text, 12, fill);
  }

  SVGrenderInfo(): svg(nullptr), cur(nullptr) {}
  SVGrenderInfo(std::filesystem::path fn):
      svg(new SVG(fn)), cur(nullptr) {}
  ~SVGrenderInfo() { delete svg; }
};

//===========================================================================================================================
//=: Node Widths :===========================================================================================================
//===========================================================================================================================

int AST_Node::own_width() { return content.length()*8 + 16; }
int AST_Node_Type::own_width() { return dec_type.toString().length()*8 + 16; }
int AST_Node_Definition::own_width() { return def->name.length()*8 + 16; }
int AST_Node_Subscript::own_width() { return 24; }
int AST_Node::own_height() { return own_width(); }
int AST_Node_Cast::own_height() { return 24; }
int AST_Node_Type::own_height() { return 24; }
int AST_Node_Definition::own_height() { return 24; }

//===========================================================================================================================
//=: SVG Renderers :=========================================================================================================
//===========================================================================================================================

  void AST_Node::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    svg->draw_circle(nid,x,y,own_width()/2,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
  }
  void AST_Node_Type::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++, r = own_width()/2;
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF1040C0,2);
    svg->draw_text(nid,x,y+4,dec_type.toString());
  }
  void AST_Node_Definition::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++, r = own_width()/2;
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF804010,2);
    svg->draw_text(nid,x,y+4,def->name);
  }
  void AST_Node_Unary::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int xx = x, yy = y+own_height()/2+16+(operand?operand->own_height()/2:0);

    svg->draw_line(nid,'m',x,y,xx,yy);
    svg->draw_circle(nid,x,y,own_width()/2,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (operand)
      operand->toSVG(xx,yy,svg);
  }
  void AST_Node_sizeof::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int xx = x, yy = y+own_height()/2+16+(operand?operand->own_height()/2:0);

    int r = own_width()/2;
    svg->draw_line(nid,'m',x,y,xx,yy);
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (operand)
      operand->toSVG(xx,yy,svg);
  }
  void AST_Node_Cast::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int xx = x, yy = y+own_height()/2+16+(operand?operand->own_width()/2:0);

    int r = own_width()/2;
    svg->draw_line(nid,'m',x,y,xx,yy);
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (operand)
      operand->toSVG(xx,yy,svg);
  }
  void AST_Node_Binary::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int lw = (left?left->width():0), rw = (right?right->width():0), shift = (lw - rw) / 2;
    int lx = x - lw/2 + shift - 12, rx = x + rw/2 + shift + 12,
         y2l = y+own_height()/2 + 16 + (left?left->own_height()/2:0),
         y2r = y+own_height()/2 + 16 + (right?right->own_height()/2:0);
    svg->draw_line(nid,'l',x,y,lx,y2l);
    svg->draw_line(nid,'r',x,y,rx,y2r);
    svg->draw_circle(nid,x,y,own_width()/2,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (left)
      left->toSVG(lx,y2l,svg);
    if (right)
      right->toSVG(rx,y2r,svg);
  }
  void AST_Node_Ternary::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2;
    int ew = exp?exp->width():0, lw = left?left->width():0, rw = right?right->width():0, eow = exp?exp->own_width():0;
    int ex = x - (ew+24+lw+24+rw)/2 + ew/2 + eow/2, lx = ex + ew/2 + 12 + lw/2, rx = lx + lw/2 + 12 + rw/2,
         y2 = y+r+16+max(max(exp?exp->own_width()/2:0, left?left->own_width()/2:0), right?right->own_width()/2:0);
    svg->draw_line(nid,'e',x,y,ex,y2);
    svg->draw_line(nid,'l',x,y,lx,y2);
    if (right or (left and svg->cur == this))
      svg->draw_line(nid,'r',x,y,rx,y2);
    svg->draw_circle(nid,x,y,r,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (exp)
      exp->toSVG(ex,y2,svg);
    if (left)
      left->toSVG(lx,y2,svg);
    if (right)
      right->toSVG(rx,y2,svg);
  }
  void AST_Node_Parameters::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2;
    int tw = width();
    int xx = x - tw/2, yy = y+r+16;
    for (size_t i = 0; i < params.size(); ++i) {
      int w = params[i]->width(), r_2 = params[i]->own_width()/2;
      xx += w/2;
      svg->draw_line(nid,'a' + i,x,y,xx,yy+r_2);
      params[i]->toSVG(xx,yy+r_2,svg);
      xx += w/2 + 24;
    }
    svg->draw_circle(nid,x,y,r,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,"()");
  }
  void AST_Node_TempInst::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2 + 8;
    int tw = width();
    int xx = x - tw/2, yy = y+r+16;
    for (size_t i = 0; i < params.size(); ++i) {
      int w = params[i]->width(), r_2 = params[i]->own_width()/2;
      xx += w/2;
      svg->draw_line(nid,'a' + i,x,y,xx,yy+r_2);
      params[i]->toSVG(xx,yy+r_2,svg);
      xx += w/2 + 24;
    }
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content + "<>");
  }
  void AST_Node_TempKeyInst::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2 + 8;
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content + "<" + key.toString() + ">");
  }
  void AST_Node_Array::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2;
    int tw = width();
    int xx = x - tw/2, yy = y+r+16;
    for (size_t i = 0; i < elements.size(); ) {
      int w = elements[i]->width(), r_2 = elements[i]->own_width()/2;
      xx += w/2;
      svg->draw_line(nid,'a' + i,x,y,xx,yy+r_2);
      elements[i]->toSVG(xx,yy+r_2,svg);
      xx += w/2 + 24;
    }
    svg->draw_circle(nid,x,y,r,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,"{}");
  }
  void AST_Node_new::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int xx = x, yy = y+own_height()/2+16+(bound?bound->own_width()/2:0);

    content = (position?"new() ":"new ") + alloc_type.toString();
    if (bound) content += "[]";
    int r = own_width()/2;
    svg->draw_line(nid,'m',x,y,xx,yy);
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (bound)
      bound->toSVG(xx,yy,svg);
  }
  void AST_Node_delete::toSVG(int x, int y, SVGrenderInfo *svg) {
    const int nid = svg->nodes_written++;
    int xx = x, yy = y+own_height()/2+16+(operand?operand->own_height()/2:0);

    content = array?"delete[]":"delete";
    int r = own_width()/2;
    svg->draw_line(nid,'m',x,y,xx,yy);
    svg->draw_rectangle(nid,x-r,y-12,x+r,y+12,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (operand)
      operand->toSVG(xx,yy,svg);
  }
  void AST_Node_Subscript::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int lw = (left?left->width():0), rw = (index?index->width():0), shift = (lw - rw) / 2;
    int lx = x - lw/2 + shift - 12, rx = x + rw/2 + shift + 12,
         y2l = y+own_height()/2 + 16 + (left?left->own_height()/2:0),
         y2r = y+own_height()/2 + 16 + (index?index->own_height()/2:0);
    svg->draw_line(nid,'l',x,y,lx,y2l);
    svg->draw_line(nid,'r',x,y,rx,y2r);
    svg->draw_circle(nid,x,y,own_width()/2,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,"[]");
    if (left)
      left->toSVG(lx,y2l,svg);
    if (index)
      index->toSVG(rx,y2r,svg);
  }


  //===========================================================================================================================
  //=: Base Call :=============================================================================================================
  //===========================================================================================================================

  void AST::writeSVG(std::filesystem::path filename) {
    jdi::SVGrenderInfo svg(filename);
    svg.cur = nullptr;
    if (!svg.svg->is_open()) return;

    int w, h;

    #ifdef DEBUG_MODE
      if (root)
        w = max(root->width(), (int)expression.length()*8)+8, h = root->height()+8 + 16;
      else
        w = (int)expression.length()*8, h = 8 + 16;
    #else
      if (root)
        w = root->width()+8, h = root->height()+8;
      else
        w = 8, h = 8;
    #endif

    svg.svg->write_header(w,h);
    if (root)
      root->toSVG(w/2, root->own_height()/2+4, &svg);
    #ifdef DEBUG_MODE
    svg.svg->draw_text("Expression",w/2,h-8,expression);
    #endif
    svg.svg->close();
  }


  //===========================================================================================================================
  //=: Recursive Width/Height Resolvers :======================================================================================
  //===========================================================================================================================

  int AST_Node            ::width()  { return own_width(); }
  int AST_Node_Binary     ::width()  { return 24 + (left?left->width():0) + (right?right->width():0); }
  int AST_Node_Unary      ::width()  { return operand?max(operand->width(),own_width()):own_width(); }
  int AST_Node_Ternary    ::width()  { return 24 + ((exp?exp->width():0) + (left?left->width():0) + (right? 24 + right->width():0)); }
  int AST_Node_Parameters ::width()  { int res = -24; for (size_t i = 0; i < params.size(); i++) res += 24 + params[i]->width(); return max(own_width(), res); }
  int AST_Node_TempInst   ::width()  { int res = -24; for (size_t i = 0; i < params.size(); i++) res += 24 + params[i]->width(); return max(own_width() + 16, res); }
  int AST_Node_TempKeyInst::width()  { return ((temp? temp->name.length() : 0) + 1 + (key.toString().length()) + 1) * 8 + 16; }
  int AST_Node_Array      ::width()  { int res = -24; for (size_t i = 0; i < elements.size(); i++) res += 24 + elements[i]->width(); return max(own_width(), res); }
  int AST_Node_Subscript  ::width()  { return 24 + (left?left->width():0) + (index?index->width():0); }
  int AST_Node            ::height() { return own_height(); }
  int AST_Node_Binary     ::height() { return max((left?left->height():0), (right?right->height():0)) + 16 + own_height(); }
  int AST_Node_Unary      ::height() { return own_height() + (operand? 16 + operand->height():0); }
  int AST_Node_Cast       ::height() { return 24 + (operand? 16 + operand->height():0); }
  int AST_Node_Ternary    ::height() { return own_height() + 16 + max(max((exp?exp->height():0), (left?left->height():0)), (right?right->height():0)); }
  int AST_Node_Parameters ::height() { int mh = 0; for (size_t i = 0; i < params.size(); i++) mh = max(mh,params[i]->height()); return own_width() + 16 + mh; }
  int AST_Node_TempInst   ::height() { int mh = 0; for (size_t i = 0; i < params.size(); i++) mh = max(mh,params[i]->height()); return own_width() + 16 + mh; }
  int AST_Node_TempKeyInst::height() { return own_height(); }
  int AST_Node_Array      ::height() { int mh = 0; for (size_t i = 0; i < elements.size(); i++) mh = max(mh,elements[i]->height()); return own_width() + 16 + mh; }
  int AST_Node_Subscript  ::height() { return max((left?left->height():0), (index?index->height():0)) + 16 + own_height(); }


//===========================================================================================================================
//=: Basic Tree Print :======================================================================================================
//===========================================================================================================================

string AST_Node::toString() const {
  return content;
}
string AST_Node_Unary::toString() const {
  return content + operand->toString();
}
string AST_Node_Binary::toString() const {
  return "(" + (left? left->toString(): "...") + ") "
         + content
         + " (" + (right? right->toString() : "...") + ")";
}
string AST_Node_Ternary::toString() const {
  return "(" + (exp ? exp->toString() : "...") + ") "
         "? (" + (left?left->toString():"...") + ") "
         ": (" + (right?right->toString():"...") + ")";
}
string AST_Node_Parameters::toString() const {
  string res = "(" + (func?func->toString():"...") + ")(";
  for (size_t i = 0; i < params.size(); ++i) {
    res += params[i]->toString();
    if (i + 1 < params.size()) res += ", ";
  }
  return res + ")";
}
string AST_Node_Cast::toString() const {
  return "(" + cast_type.toString() + ")(" + operand->toString() + ")";
}
string AST_Node_Definition::toString() const {
  return def? def->name : "...";
}
string AST_Node_Scope::toString() const {
  return (left?left->toString() : "...") + "::" + (right?right->content:"???");
}
string AST_Node_sizeof::toString() const {
  return "sizeof(" + operand->toString() + ")";
}
string AST_Node_Subscript::toString() const {
  return "(" + (left? left->toString() : "...") + ")"
         "[" + (index? index->toString() : "...") + "]";
}
string AST_Node_Type::toString() const {
  return dec_type.toString();
}
string AST_Node_Array::toString() const {
  string res = "{ ";
  for (size_t i = 0; i < elements.size(); ++i)
    res += elements[i]->toString() + (i + 1 < elements.size()? ", " : " ");
  return res + "}";
}
string AST_Node_new::toString() const {
  string res = (position)? "new(" + position->toString() + ") " : "new ";
  res += alloc_type.toString();
  if (bound) res += "[" + bound->toString() + "]";
  return res;
}
string AST_Node_delete::toString() const {
  return (array?"delete ":"delete[] ") + operand->toString();
}
string AST_Node_TempInst::toString() const {
  string res = temp? temp->toString() + "<" : "(<nullptr TEMPLATE>)<";
  for (size_t i = 0; i < params.size(); ++i) {
    res += params[i]->toString();
    if (i+1<params.size()) res += ", ";
  }
  return res + ">";
}
string AST_Node_TempKeyInst::toString() const {
  return (temp ? temp->name + "<" : "(<nullptr TEMPLATE>)<")
               + key.toString() + ">";
}

}  // namespace jdi
