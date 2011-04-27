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

enum { o_incomplete = 0, o_widget = 1, o_container = 2 };

struct gtkl_object
{
  int id; // The ID by which we will refer to this instance
  const int type; // What kind of objcet we are.
  int srw, srh; // Size request dimensions
  virtual void resolve() {}
  virtual void reposition() {}
  
  gtkl_object(): type(o_incomplete) {}
  gtkl_object(int t): type(t), srw(0), srh(0) { }
  gtkl_object(int w, int h, int t): type(t), srw(w), srh(h) { }
  gtkl_object(int id, int w, int h, int t): id(id), type(t), srw(w), srh(h) { }
  virtual ~gtkl_object() {}
};
struct gtkl_placer: gtkl_object
{
  int x,y,w, h;  // My metrics, relative to my parent
  int parent_id; // The ID of my parent
  char child_id; // My ID according to my parent
  virtual void resolve() { x=4, y=4, srw = 2, srh = 2; }
  virtual void resize(int xr, int yr, int wr, int hr) { x=xr, y=yr, srw = wr, srh = hr; }
  gtkl_placer(int t): gtkl_object(t) {}
  gtkl_placer(int w, int h, int t): gtkl_object(w,h,t) {}
  gtkl_placer(int id,int w, int h, int t): gtkl_object(id,w,h,t) {}
};
struct gtkl_widget: gtkl_placer
{
  ui_handle me; // The handle the OS knows my control by
  int actions; // Action messages received from OS, such as click
  long attribs; // Anything the API needs to know
  gtkl_widget(int w, int h): gtkl_placer(w,h,o_widget), me(NULL), actions(0) {}
  gtkl_widget(ui_handle me, int w, int h): gtkl_placer(w,h,o_widget), me(me), actions(0) {}
  gtkl_widget(int id,ui_handle me, int w, int h): gtkl_placer(id,w,h,o_widget), me(me), actions(0), attribs(0) {}
  gtkl_widget(int id,ui_handle me, int at, int w, int h): gtkl_placer(id,w,h,o_widget), me(me), actions(0), attribs(at) {}
  void resize(int,int,int,int); // IMPLEMENT this
};
struct gtkl_container: gtkl_placer
{
  gtkl_container(int w, int h):  gtkl_placer(w,h,o_container) {}
  gtkl_container(int w, int h, int t):  gtkl_placer(w,h,t) {}
  gtkl_container(int id,int w, int h, int t):  gtkl_placer(id,w,h,t) {}
  virtual void clear() {}
  virtual void remove(char cid) {}
  virtual void resize(int xr, int yr, int wr, int hr) { printf("Error: Mindless resize of widget %d\n",id); }
};


#include <map>
inline int min(int x, int y) { return x<y?x:y; }
// Above we have the basic class layout;
// Now we provide some useful algorithms
struct gtkl_table: gtkl_container
{
  short gsx, gsy; // Grid Size: x and y
  struct attachment {
    int x,y,r,b;
    gtkl_placer* child;
    
    struct dc { 
      int x,y,w,h;
      dc(int x,int y,int w,int h): x(x), y(y), w(w), h(h) {}
    } d; // This represents draw coordinates
    
    attachment(): x(0), y(0), r(1),b(1), child(NULL), d(0,0,0,0) {}
    attachment(int x, int y, int r, int b, gtkl_placer* ch): x(x), y(y), r(r), b(b), child(ch), d(2,2,4,4) {};
  };
  std::map<char,attachment> atts;
  typedef std::map<char,attachment>::iterator atti; // Iterator
  typedef std::pair<char,attachment> attp; // Key-value pair
  typedef std::pair<atti,bool> attn; // Newly inserted item return pair (via insert())
  
  bool reserved(char cid)
  {
    return atts.find(cid) != atts.end();
  }
  bool attach(gtkl_placer* whom, int l, int r, int t, int b, char cid)
  {
    attachment att(l,t,r,b,whom);
    attn insd = atts.insert(attp(cid,att));
    whom->parent_id = id; whom->child_id = cid;
    return insd.second;
  }
  void resize(int xnew, int ynew, int dwid, int dhgt)
  {
    x=xnew, y=ynew, w=dwid, h=dhgt;
    int cw = dwid / gsx, ch = dhgt / gsy;
    if (ch<gsy) ch=gsy; if (cw<gsx) cw=gsx;
    
    int givex[gsx][gsy], givey[gsx][gsy];
    // Make all occupied cells yield max give
    for (int i = 0; i < gsx; i++)
      for (int ii = 0; ii < gsy; ii++)
        givex[i][ii] = cw, givey[i][ii] = ch;
    for (atti mi = atts.begin(); mi != atts.end(); mi++)
    {
      // Make all allocated cells not give at all
      for (int xz = mi->second.x; xz < mi->second.r; xz++)
      for (int yz = mi->second.y; yz < mi->second.b; yz++)
        givex[xz][yz] = givey[xz][yz] = 0;
      // Make not-completely-occupied allocated cells have max give
      for (int xz = mi->second.x; xz < mi->second.r; xz++)
      for (int yz = mi->second.y + (mi->second.child->srh+8)/ch; yz < mi->second.b; yz++)
        givey[xz][yz] = ch;
      for (int yz = mi->second.y; yz < mi->second.b; yz++)
      for (int xz = mi->second.x + (mi->second.child->srw+8)/cw; xz < mi->second.r; xz++)
        givex[xz][yz] = cw;
      // Decrease vertical give (for each horizontal unit)
      for (int xz = min(mi->second.x + (mi->second.child->srw+8)/cw, mi->second.r-1), yz = mi->second.y; yz < mi->second.b; yz++)
        givex[xz][yz] = cw*(xz+1-mi->second.x) - (mi->second.child->srw+8);
      // Decrease horizontal give (for each vertical unit)
      for (int yz = min(mi->second.y + (mi->second.child->srh+8)/ch, mi->second.b-1), xz = mi->second.x; xz < mi->second.r; xz++)
        givey[xz][yz] = ch*(yz+1-mi->second.y) - (mi->second.child->srh+8);
      //cons_show_message("");
    }
    
    // Lay everything out as-is first, ignoring overlap.
    for (atti mi = atts.begin(); mi != atts.end(); mi++)
      mi->second.d.x = mi->second.x*cw, mi->second.d.y = mi->second.y*ch, mi->second.d.w = (mi->second.child->srw+8), mi->second.d.h = (mi->second.child->srh+8);
    
    // Expand grid where absolutely necessary
    int maxgx[gsx], maxgy[gsy]; //Compute resize allotment
    for (int xx = 0; xx < gsx; xx++)
      maxgx[xx] = cw;
    for (int yy = 0; yy < gsy; yy++)
      maxgy[yy] = ch;
    for (int xx = 0; xx < gsx; xx++)
      for (int yy = 0; yy < gsy; yy++)
      {
        if (givex[xx][yy] < maxgx[xx])
          maxgx[xx] = givex[xx][yy];
        if (givey[xx][yy] < maxgy[xx])
          maxgy[xx] = givey[xx][yy];
      }
    
    int losx = 0, losy = 0; // Total left-over space (always <= 0)
    for (int i = 0; i < gsx; i++)
      losx += maxgx[i];
    for (int i = 0; i < gsy; i++)
      losy += maxgy[i];
    for (atti mi = atts.begin(); mi != atts.end(); mi++)
      mi->second.child->resize(x + mi->second.d.x, y + mi->second.d.y, mi->second.d.w, mi->second.d.h);
    
    // Expand items to fill new bounds
    for (atti mi = atts.begin(); mi != atts.end(); mi++)
    {
      for (int yy = mi->second.y, xx = mi->second.x; xx < mi->second.r; xx++)
        mi->second.d.w += givex[xx][yy];
      for (int xx = mi->second.x, yy = mi->second.y; yy < mi->second.b; yy++)
        mi->second.d.h += givey[xx][yy];
    }
    for (atti mi = atts.begin(); mi != atts.end(); mi++)
      mi->second.child->resize(x + mi->second.d.x, y + mi->second.d.y, mi->second.d.w, mi->second.d.h);
  }
  void resolve()
  {
    srw = 0; srh = 0;
    
    for (atti mi = atts.begin(); mi != atts.end(); mi++)
      mi->second.child->resolve();
    
    // Calculate minimal X size
    for (int xx = 0; xx < gsx; xx++)
    {
      int tc = 0;
      for (atti mi = atts.begin(); mi != atts.end(); mi++)
        if (mi->second.x <= xx and mi->second.r > xx)
          tc += mi->second.child->srh;
      if (tc > srh)
        srh = tc;
    }
    // Calculate minimal Y size
    for (int yy = 0; yy < gsy; yy++)
    {
      int tr = 0;
      for (atti mi = atts.begin(); mi != atts.end(); mi++)
        if (mi->second.y <= yy and mi->second.b > yy)
          tr += mi->second.child->srw;
      if (tr > srw)
        srw = tr;
    }
    srw += 8, srh += 8;
  }
  
  gtkl_table(int ch, int cw): gtkl_container(cw*4,ch*4,false), gsx(cw), gsy(ch) { }
  ~gtkl_table() {}
} mytable(3,3);
/*
void gtklcreate()
{
  mytable.attach(new gtkl_widget(64,32), 0, 2, 0, 1);
  mytable.attach(new gtkl_widget(32,64), 2, 3, 0, 2);
  mytable.attach(new gtkl_widget(64,32), 1, 3, 2, 3);
  mytable.attach(new gtkl_widget(48,64), 0, 1, 1, 3);
}
void gtklstep()
{
}
void gtkldraw()
{
  mytable.resolve();
  
  int dwid = (mouse_x - 64) - mytable.srw; dwid /= mytable.w;
  int dhgt = (mouse_y - 64) - mytable.srh; dhgt /= mytable.h;
  for (int x = 0, xx = 64; x < mytable.w; xx += mytable.cols[x++] + dwid)
  {
    for (int y = 0, yy = 64; y < mytable.h; yy += mytable.rows[y++] + dhgt)
      draw_set_color(((long)mytable.children[x][y]&0xFFF) * 29 << 8),
      draw_rectangle(xx, yy, xx + mytable.cols[x]+ dwid, yy + mytable.rows[y] + dhgt,0);
  }
  for (int x = 0, xx = 64; x < mytable.w; xx += mytable.cols[x++] + dwid)
  {
    for (int y = 0, yy = 64; y < mytable.h; yy += mytable.rows[y++] + dhgt)
      draw_set_color(random(c_white)),
      draw_rectangle(xx, yy, xx + mytable.cols[x]+ dwid, yy + mytable.rows[y] + dhgt,1);
  }
  draw_ellipse(0,0,mouse_x,mouse_y,1);
}
*/

