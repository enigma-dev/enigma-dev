/**
 * @file AST_Export.cpp
 * @brief Source implementing export functions for printing and rendering ASTs.
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

#include "AST.h"
#include <General/svg_simple.h>

namespace jdi {
  void AST::print() {
    #ifdef DEBUG_MODE
      cout << expression << endl;
    #endif
    if (root) root->print();
    cout << "   Current={" << (current?current->content:"NULL") << "}" << endl;
  }
  
  AST::AST(): root(NULL), current(NULL) {
    
  }
  
  /// A wrapper to \c SVG which generates IDs based on an internally-stored node count.
  struct AST::SVGrenderInfo {
    SVG *svg;
    AST_Node *cur;
    int nodes_written;
    
    void draw_circle(int nid,int x,int y,int r,unsigned fill,unsigned stroke = 0xFF000000,int stroke_width = 2) { svg->draw_circle("Node"+svg->tostring(nid),x,y,r,fill,stroke,stroke_width); }
    void draw_line(int nid,char s_id,int x1,int y1,int x2,int y2,unsigned stroke = 0xFF000000,int stroke_width = 2) { svg->draw_line("Connector_"+svg->tostring(nid)+"_"+s_id,x1,y1,x2,y2,stroke,stroke_width); }
    void draw_text(int nid,int cx,int bly,string t,unsigned fill = 0xFF000000) { svg->draw_text("Label_"+svg->tostring(nid),cx,bly,t,12,fill); }
    
    SVGrenderInfo(): svg(NULL), cur(NULL), nodes_written(0) {}
    SVGrenderInfo(const char* fn): svg(new SVG(fn)), cur(NULL), nodes_written(0) {}
    ~SVGrenderInfo() { delete svg; }
  };
  
  
  //===========================================================================================================================
  //=: Node Widths :===========================================================================================================
  //===========================================================================================================================
  
  int AST::AST_Node::own_width() { return content.length()*8 + 16; }
  int AST::AST_Node_Group::own_width() { return 32; }
  
  
  //===========================================================================================================================
  //=: SVG Renderers :=========================================================================================================
  //===========================================================================================================================
  
  void AST::AST_Node::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    svg->draw_circle(nid,x,y,own_width()/2,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
  }
  void AST::AST_Node_Unary::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2;
    int xx = x, yy = y+r+16+(right?right->own_width()/2:0);
    
    svg->draw_line(nid,'m',x,y,xx,yy);
    svg->draw_circle(nid,x,y,r,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (right)
      right->toSVG(xx,yy,svg);
  }
  void AST::AST_Node_Binary::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2;
    int lw = (left?left->width():0), rw = (right?right->width():0), shift = (lw - rw) / 2;
    int lx = x - lw/2 + shift - 12, rx = x + rw/2 + shift + 12,
         y2 = y+r+16+max(left?left->own_width()/2:0,right?right->own_width()/2:0);
    svg->draw_line(nid,'l',x,y,lx,y2);
    svg->draw_line(nid,'r',x,y,rx,y2);
    svg->draw_circle(nid,x,y,r,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,content);
    if (left)
      left->toSVG(lx,y2,svg);
    if (right)
      right->toSVG(rx,y2,svg);
  }
  void AST::AST_Node_Ternary::toSVG(int x, int y, SVGrenderInfo *svg)
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
  void AST::AST_Node_Group::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2;
    int xx = x, yy = y+r+16+(root?root->own_width()/2:0);
    
    svg->draw_line(nid,'m',x,y,xx,yy);
    svg->draw_circle(nid,x,y,r,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,"()");
    if (root)
      root->toSVG(xx,yy,svg);
  }
  void AST::AST_Node_Parameters::toSVG(int x, int y, SVGrenderInfo *svg)
  {
    const int nid = svg->nodes_written++;
    int r = own_width()/2;
    int tw = width();
    int xx = x - tw/2, yy = y+r+16;
    for (size_t i = 0; i < params.size(); ) {
      int w = params[i]->width(), r_2 = params[i]->own_width()/2;
      xx += w/2;
      svg->draw_line(nid,'a' + i,x,y,xx,yy+r_2);
      params[i]->toSVG(xx,yy+r_2,svg);
      xx += w/2 + 24;
    }
    svg->draw_circle(nid,x,y,r,0xFFFFFFFF,svg->cur == this ? 0xFF00C000 : 0xFF000000,2);
    svg->draw_text(nid,x,y+4,"()");
  }
  
  
  //===========================================================================================================================
  //=: Base Call :=============================================================================================================
  //===========================================================================================================================
  
  void AST::writeSVG(const char* filename) {
    SVGrenderInfo svg(filename);
    svg.cur = current;
    if (!svg.svg->is_open()) return;
    
    int w = root->width()+8, h = root->height()+8;
    #ifdef DEBUG_MODE 
      h += 16;
    #endif
    
    svg.svg->write_header(w,h);
    root->toSVG(w/2, root->own_width()/2+4, &svg);
    #ifdef DEBUG_MODE 
    svg.svg->draw_text("Expression",w/2,h-8,expression);
    #endif
    svg.svg->close();
  }
  
  
  //===========================================================================================================================
  //=: Recursive Width/Height Resolvers :======================================================================================
  //===========================================================================================================================
  
  int AST::AST_Node::width() { return own_width(); }
  int AST::AST_Node_Binary::width() { return 24 + (left?left->width():0) + (right?right->width():0); }
  int AST::AST_Node_Unary::width() { return right?max(right->width(),own_width()):own_width(); }
  int AST::AST_Node_Ternary::width() { return 24 + ((exp?exp->width():0) + (left?left->width():0) + (right? 24 + right->width():0)); }
  int AST::AST_Node_Group::width() { return root?max(root->width(),own_width()):own_width(); }
  int AST::AST_Node_Parameters::width() { int res = -24; for (size_t i = 0; i < params.size(); i++) res += 24 + params[i]->width(); return max(own_width(), res); }
  int AST::AST_Node::height() { return own_width(); }
  int AST::AST_Node_Binary::height() { return max((left?left->height():0), (right?right->height():0)) + 16 + own_width(); }
  int AST::AST_Node_Unary::height() { return own_width() + (right?16 + right->height():0); }
  int AST::AST_Node_Ternary::height() { return own_width() + 16 + max(max((exp?exp->height():0), (left?left->height():0)), (right?right->height():0)); }
  int AST::AST_Node_Group::height() { return own_width() + (root?16 + root->height():0); }
  int AST::AST_Node_Parameters::height() { int mh = 0; for (size_t i = 0; i < params.size(); i++) mh = max(mh,params[i]->height()); return own_width() + 16 + mh; }
  
  
  //===========================================================================================================================
  //=: Basic Tree Print :======================================================================================================
  //===========================================================================================================================
  
  void AST::AST_Node::print() { cout << "(" << content << ")"; }
  void AST::AST_Node_Unary::print() {}
  void AST::AST_Node_Binary::print() { cout << "( " << content << " )["; if (left) left->print(); else cout << "(...)"; if (right) right->print(); else cout << "(...)"; cout << "]"; }
  void AST::AST_Node_Ternary::print() {}
  void AST::AST_Node_Group::print() { cout << "("; if (root) root->print(); else cout << "..."; cout << ")" << endl; }
  void AST::AST_Node_Parameters::print() {}
}
