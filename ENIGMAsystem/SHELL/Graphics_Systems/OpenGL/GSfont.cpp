/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura, Harijs Grînbergs                           **
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

#include <math.h>
#include <string>
#include "OpenGLHeaders.h"
#include "GScolors.h"

using namespace std;
#include "../../Universal_System/fontstruct.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma {
  static int currentfont = -1;
  extern unsigned bound_texture;
}

using namespace enigma;

void draw_text(int x,int y,string str)
{
  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);

  int xx = x, yy = y+fnt->height;
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      xx = x, yy += fnt->height, i += str[i+1] == '\n';
    else if (str[i] == '\n')
      xx = x, yy += fnt->height;
    else if (str[i] == ' ')
      xx += fnt->height/3; // FIXME: what's GM do about this?
    else
    {
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
        glTexCoord2f(g.tx,  g.ty);
          glVertex2i(xx + g.x,  yy + g.y);
        glTexCoord2f(g.tx2, g.ty);
          glVertex2i(xx + g.x2, yy + g.y);
        glTexCoord2f(g.tx2, g.ty2);
          glVertex2i(xx + g.x2, yy + g.y2);
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2i(xx + g.x,  yy + g.y2);
      xx += g.xs;
    }
  }
  glEnd();
}
///////////////////////////////////////////////////
int string_width_line(string str, int line)
{
  const font *const fnt = fontstructarray[currentfont];
  int len = 0, cl = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r' or str[i] == '\n'){
      if (cl == line)
        return len;
      cl += 1;
      len = 0;
    }else if (str[i] == ' ')
      len += fnt->height/3; // FIXME: what's GM do about this?
    else {
      len += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
    }
  }
  return len;
}

unsigned int string_width_ext_line(string str, int w, int line)
{
  font *fnt = fontstructarray[currentfont];

  unsigned int width = 0, tw = 0; int cl = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r' or str[i] == '\n')
      if (cl == line) return width; else width = 0, cl +=1;
    else if (str[i] == ' '){
      width += fnt->height/3, tw = 0;
      for (unsigned c = i+1; c < str.length(); c++) //This is getting messy
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        tw += fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount].xs;
      }
      if (width+tw >= unsigned(w) && w != -1)
        if (cl == line) return width; else width = 0, cl +=1;
      else;
    }else
      width += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
  }
  return width;
}

unsigned int string_width_ext_line_count(string str, int w)
{
  font *fnt = fontstructarray[currentfont];

  unsigned int width = 0, tw = 0, cl = 1;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r' or str[i] == '\n')
      width = 0, cl +=1;
    else if (str[i] == ' '){
      width += fnt->height/3, tw = 0;
      for (unsigned c = i+1; c < str.length(); c++) //This is getting messy
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        tw += fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount].xs;
      }
      if (width+tw >= unsigned(w) && w != -1)
        width = 0, cl +=1;
    }else
      width += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
  }
  return cl;
}
///////////////////////////////////////////////////////
//The following is certainly not pretty, but this is the best way I thought of to replicate GM's function
void draw_text_ext(int x,int y,string str, int sep, int w)
{
  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);

  int xx = x, yy = y+fnt->height, width = 0, tw = 0;
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      xx = x, yy +=  (sep+2 ? fnt->height : sep), i += str[i+1] == '\n';
    else if (str[i] == '\n')
      xx = x, yy += (sep+2 ? fnt->height : sep);
    else if (str[i] == ' '){
      xx += fnt->height/3, width = xx-x;
      tw = 0;
      for (unsigned c = i+1; c < str.length(); c++) //This is getting messy
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
        tw += g.xs;
      }

      if (width+tw >= w && w != -1)
        xx = x, yy += (sep==-1 ? fnt->height : sep), width = 0, tw = 0;
    }else{
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
        glTexCoord2f(g.tx,  g.ty);
          glVertex2i(xx + g.x,  yy + g.y);
        glTexCoord2f(g.tx2, g.ty);
          glVertex2i(xx + g.x2, yy + g.y);
        glTexCoord2f(g.tx2, g.ty2);
          glVertex2i(xx + g.x2, yy + g.y2);
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2i(xx + g.x,  yy + g.y2);
      xx += g.xs;
      width = xx-x;
    }
  }
  glEnd();
}

void draw_text_transformed(double x,double y,string str,double xscale,double yscale,double rot)
{
  if (currentfont == -1)
    return;

  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);

  rot *= M_PI/180;

  const float sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = fnt->height/3 * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  float xx = x + shi, yy = y + chi;

  int lines = 1, w;
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi, i += str[i+1] == '\n';
    else if (str[i] == '\n')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi;
    else if (str[i] == ' ')
      xx += sw,
      yy -= sh;
    else
    {
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
      w = g.x2-g.x;
        const float lx = xx + g.y * svy;
        const float ly = yy + g.y * cvy;

        glTexCoord2f(g.tx,  g.ty);
          glVertex2f(lx, ly);
        glTexCoord2f(g.tx2, g.ty);
          glVertex2f(lx + w * cvx, ly - w * svx);

        glTexCoord2f(g.tx2, g.ty2);
          glVertex2f(xx + w * cvx + g.y2 * svy, yy - w * svx + g.y2 * cvy);
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2f(xx + g.y2 * svy,  yy + g.y2 * cvy);

      xx += g.xs * cvx;
      yy -= g.xs * svx;
    }
  }
  glEnd();
}

void draw_text_ext_transformed(double x,double y,string str,int sep, int w, double xscale,double yscale,double rot)
{
  if (currentfont == -1)
    return;

  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);

  rot *= M_PI/180;

  const float sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = fnt->height/3 * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  float xx = x + shi, yy = y + chi, wi;

  int lines = 1,width = 0, tw = 0;
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi, i += str[i+1] == '\n';
    else if (str[i] == '\n')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi;
    else if (str[i] == ' '){
      xx += sw,
      yy -= sh;

      width += fnt->height/3;
      tw = 0;
      for (unsigned c = i+1; c < str.length(); c++)
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
        tw += g.xs;
      }

      if (width+tw >= w && w != -1)
        lines += 1, xx = x + lines * shi, yy = y + lines * chi, width = 0, tw = 0;
    }else
    {
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
      wi = g.x2-g.x;
        const float lx = xx + g.y * svy;
        const float ly = yy + g.y * cvy;

        glTexCoord2f(g.tx,  g.ty);
          glVertex2f(lx, ly);
        glTexCoord2f(g.tx2, g.ty);
          glVertex2f(lx + wi * cvx, ly - wi * svx);

        glTexCoord2f(g.tx2, g.ty2);
          glVertex2f(xx + wi * cvx + g.y2 * svy, yy - wi * svx + g.y2 * cvy);
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2f(xx + g.y2 * svy,  yy + g.y2 * cvy);


      xx += g.xs * cvx;
      yy -= g.xs * svx;
      width += g.xs;
    }
  }
  glEnd();
}

void draw_text_transformed_color(double x,double y,string str,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a)
{
  if (currentfont == -1)
    return;

  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);

  rot *= M_PI/180;

  const float sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = fnt->height/3 * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  float xx = x + shi, yy = y + chi, tx1, tx2, width = 0;

  int lines = 1, w, hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4, lw = string_width_line(str, lines-1);
  glPushAttrib(GL_CURRENT_BIT);
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi, width = 0, i += str[i+1] == '\n', lw = string_width_line(str, lines-1);
    else if (str[i] == '\n')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi, width = 0, lw = string_width_line(str, lines-1);
    else if (str[i] == ' '){
      xx += sw,
      yy -= sh;
      width += fnt->height/3 * xscale;
    }else{
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
      tx1 = width, tx2 = width+g.xs * xscale;
      hcol1 = merge_color(c1,c2,tx1/lw);
      hcol2 = merge_color(c1,c2,tx2/lw);
      hcol3 = merge_color(c4,c3,tx1/lw);
      hcol4 = merge_color(c4,c3,tx2/lw);
      w = g.x2-g.x;
        const float lx = xx + g.y * svy;
        const float ly = yy + g.y * cvy;

        glColor4ub(__GETR(hcol1),__GETG(hcol1),__GETB(hcol1),char(a*255));
        glTexCoord2f(g.tx,  g.ty);
          glVertex2f(lx, ly);
        glColor4ub(__GETR(hcol2),__GETG(hcol2),__GETB(hcol2),char(a*255));
        glTexCoord2f(g.tx2, g.ty);
          glVertex2f(lx + w * cvx, ly - w * svx);

        glColor4ub(__GETR(hcol3),__GETG(hcol3),__GETB(hcol3),char(a*255));
        glTexCoord2f(g.tx2, g.ty2);
          glVertex2f(xx + w * cvx + g.y2 * svy, yy - w * svx + g.y2 * cvy);
        glColor4ub(__GETR(hcol4),__GETG(hcol4),__GETB(hcol4),char(a*255));
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2f(xx + g.y2 * svy,  yy + g.y2 * cvy);

      width += g.xs * xscale;
      xx += g.xs * cvx;
      yy -= g.xs * svx;
    }
  }
  glEnd();
  glPopAttrib();
}

void draw_text_ext_transformed_color(double x,double y,string str,int sep,int w,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a)
{
  if (currentfont == -1)
    return;

  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);

  rot *= M_PI/180;

  const float sv = sin(rot), cv = cos(rot),
    svx = sv*xscale, cvx = cv*xscale, svy = sv * yscale,
    cvy = cv*yscale, sw = fnt->height/3 * cvx, sh = fnt->height/3 * svx,
    chi = fnt->height * cvy, shi = fnt->height * svy;

  float xx = x + shi, yy = y + chi, tx1, tx2, width = 0;

  int lines = 1, wi, hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4, lw = string_width_ext_line(str, w, lines-1), tw;
  glPushAttrib(GL_CURRENT_BIT);
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi, width = 0, i += str[i+1] == '\n', lw = string_width_ext_line(str, w, lines-1);
    else if (str[i] == '\n')
      lines += 1, xx = x + lines * shi, yy = y + lines * chi, width = 0, lw = string_width_ext_line(str, w, lines-1);
    else if (str[i] == ' '){
      xx += sw,
      yy -= sh;
      width += fnt->height/3 * xscale;
      tw = 0;
      for (unsigned c = i+1; c < str.length(); c++)
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
        tw += g.xs;
      }

      if (width+tw >= w && w != -1)
        lines += 1, xx = x + lines * shi, yy = y + lines * chi, width = 0, lw = string_width_ext_line(str, w, lines-1);
    }else{
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
      tx1 = width, tx2 = width+g.xs * xscale;
      hcol1 = merge_color(c1,c2,tx1/lw);
      hcol2 = merge_color(c1,c2,tx2/lw);
      hcol3 = merge_color(c4,c3,tx1/lw);
      hcol4 = merge_color(c4,c3,tx2/lw);
      wi = g.x2-g.x;
        const float lx = xx + g.y * svy;
        const float ly = yy + g.y * cvy;

        glColor4ub(__GETR(hcol1),__GETG(hcol1),__GETB(hcol1),char(a*255));
        glTexCoord2f(g.tx,  g.ty);
          glVertex2f(lx, ly);
        glColor4ub(__GETR(hcol2),__GETG(hcol2),__GETB(hcol2),char(a*255));
        glTexCoord2f(g.tx2, g.ty);
          glVertex2f(lx + wi * cvx, ly - wi * svx);

        glColor4ub(__GETR(hcol3),__GETG(hcol3),__GETB(hcol3),char(a*255));
        glTexCoord2f(g.tx2, g.ty2);
          glVertex2f(xx + wi * cvx + g.y2 * svy, yy - wi * svx + g.y2 * cvy);
        glColor4ub(__GETR(hcol4),__GETG(hcol4),__GETB(hcol4),char(a*255));
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2f(xx + g.y2 * svy,  yy + g.y2 * cvy);

      width += g.xs * xscale;
      xx += g.xs * cvx;
      yy -= g.xs * svx;
    }
  }
  glEnd();
  glPopAttrib();
}

void draw_text_color(int x,int y,string str,int c1,int c2,int c3,int c4,double a)
{
  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);


  glPushAttrib(GL_CURRENT_BIT);
  int xx = x, yy = y+fnt->height, hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4,  line = 0, sw = string_width_line(str, line);
  float tx1, tx2;
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r'){
      xx = x, yy += fnt->height, i += str[i+1] == '\n';
      line += 1;
      sw = string_width_line(str, line);
    }else if (str[i] == '\n'){
      xx = x, yy += fnt->height;
      line += 1;
      sw = string_width_line(str, line);
    }else if (str[i] == ' ')
      xx += fnt->height/3; // FIXME: what's GM do about this?
    else
    {
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
      tx1 = xx-x, tx2 = xx+g.xs-x;
      hcol1 = merge_color(c1,c2,tx1/sw);
      hcol2 = merge_color(c1,c2,tx2/sw);
      hcol3 = merge_color(c4,c3,tx1/sw);
      hcol4 = merge_color(c4,c3,tx2/sw);
        glColor4ub(__GETR(hcol1),__GETG(hcol1),__GETB(hcol1),char(a*255));
        glTexCoord2f(g.tx,  g.ty);
          glVertex2i(xx + g.x,  yy + g.y);

        glColor4ub(__GETR(hcol2),__GETG(hcol2),__GETB(hcol2),char(a*255));
        glTexCoord2f(g.tx2, g.ty);
          glVertex2i(xx + g.x2, yy + g.y);

        glColor4ub(__GETR(hcol3),__GETG(hcol3),__GETB(hcol3),char(a*255));
        glTexCoord2f(g.tx2, g.ty2);
          glVertex2i(xx + g.x2, yy + g.y2);

        glColor4ub(__GETR(hcol4),__GETG(hcol4),__GETB(hcol4),char(a*255));
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2i(xx + g.x,  yy + g.y2);
      xx += g.xs;
    }
  }
  glEnd();
  glPopAttrib();
}

void draw_text_ext_color(int x,int y,string str,int sep, int w, int c1,int c2,int c3,int c4,double a)
{
  font *fnt = fontstructarray[currentfont];

  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);

  glPushAttrib(GL_CURRENT_BIT);
  int xx = x, yy = y+fnt->height, width = 0, tw = 0, hcol1 = c1, hcol2 = c1, hcol3 = c3, hcol4 = c4,  line = 0, sw = string_width_ext_line(str, w, line);
  glBegin(GL_QUADS);
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r')
      xx = x, yy +=  (sep+2 ? fnt->height : sep), i += str[i+1] == '\n',  width = 0, line += 1, sw = string_width_ext_line(str, w, line);
    else if (str[i] == '\n')
      xx = x, yy += (sep+2 ? fnt->height : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line);
    else if (str[i] == ' '){
      xx += fnt->height/3;
      width = xx-x;
      tw = 0;
      for (unsigned c = i+1; c < str.length(); c++) //This is getting messy
      {
        if (str[c] == ' ' or str[c] == '\r' or str[c] == '\n')
          break;
        fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
        tw += g.xs;
      }

      if (width+tw >= w && w != -1)
        xx = x, yy += (sep==-1 ? fnt->height : sep), width = 0, line += 1, sw = string_width_ext_line(str, w, line);
    }else{
      fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
      hcol1 = merge_color(c1,c2,double(width)/sw);
      hcol2 = merge_color(c1,c2,double(width+g.xs)/sw);
      hcol3 = merge_color(c4,c3,double(width)/sw);
      hcol4 = merge_color(c4,c3,double(width+g.xs)/sw);
        glColor4ub(__GETR(hcol1),__GETG(hcol1),__GETB(hcol1),char(a*255));
        glTexCoord2f(g.tx,  g.ty);
          glVertex2i(xx + g.x,  yy + g.y);
        glColor4ub(__GETR(hcol2),__GETG(hcol2),__GETB(hcol2),char(a*255));
        glTexCoord2f(g.tx2, g.ty);
          glVertex2i(xx + g.x2, yy + g.y);
        glColor4ub(__GETR(hcol3),__GETG(hcol3),__GETB(hcol3),char(a*255));
        glTexCoord2f(g.tx2, g.ty2);
          glVertex2i(xx + g.x2, yy + g.y2);
        glColor4ub(__GETR(hcol4),__GETG(hcol4),__GETB(hcol4),char(a*255));
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2i(xx + g.x,  yy + g.y2);
      xx += g.xs;
      width = xx-x;
    }
  }
  glEnd();
  glPopAttrib();
}

unsigned int font_get_texture(int fnt)
{
  font *f = fontstructarray[fnt];
  return f ? f->texture : unsigned(-1);
}
unsigned int font_get_texture_width(int fnt)
{
  font *f = fontstructarray[fnt];
  return f ? f->twid: unsigned(-1);
}
unsigned int font_get_texture_height(int fnt)
{
  font *f = fontstructarray[fnt];
  return f ? f->thgt: unsigned(-1);
}

void draw_set_font(int fnt)
{
  enigma::currentfont = fnt;
}

int string_width(string str)
{
  const font *const fnt = fontstructarray[currentfont];
  int mlen = 0, tlen = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r' or str[i] == '\n')
      tlen = 0;
    else if (str[i] == ' ')
      tlen += fnt->height/3; // FIXME: what's GM do about this?
    else {
      tlen += fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount].xs;
      if (tlen > mlen) mlen = tlen;
    }
  }
  return mlen;
}

int string_height(string str)
{
  const font *const fnt = fontstructarray[currentfont];
  int hgt = fnt->height;
  for (unsigned i = 0; i < str.length(); i++)
    if (str[i] == '\r' or str[i] == '\n')
      hgt += fnt->height;
  return hgt;
}
/*
unsigned int string_height(string str = "") //this is funny argument. Even in GM it doesn't do anything, as fonts have only one height when drawn in one line
{ //So I added a default argument, so the function can be called without any (e.g. string_height())
  font *fnt = fontstructarray[currentfont];
  return fnt->height;
}*/

unsigned int string_width_ext(string str, int sep, int w) //here sep doesn't do anything, but I can't make it 'default = ""', because its the second argument
{
  font *fnt = fontstructarray[currentfont];

  unsigned int width = 0, maxwidth = 0;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == ' '){
        if (width >= unsigned(w) && w!=-1)
            (width>maxwidth ? maxwidth=width, width = 0 : width = 0);
        else
            width += fnt->height/3; // FIXME: what's GM do about this?
    }else{
        fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
        width += g.xs;
    }
  }
  return maxwidth;
}

unsigned int string_height_ext(string str, int sep, int w)
{
  font *fnt = fontstructarray[currentfont];

  unsigned int width = 0, tw = 0, height = fnt->height;
  for (unsigned i = 0; i < str.length(); i++)
  {
    if (str[i] == '\r' or str[i] == '\n')
      width = 0, height +=  (sep+2 ? fnt->height : sep);
    else if (str[i] == ' '){
      width += fnt->height/3;
      tw = 0;
      for (unsigned c = i+1; c < str.length(); c++) //This is getting messy
      {
        if (str[c] == ' ' or str[i] == '\r' or str[i] == '\n')
          break;
        fontglyph &g = fnt->glyphs[(unsigned char)(str[c] - fnt->glyphstart) % fnt->glyphcount];
        tw += g.xs;
      }

      if (width+tw >= unsigned(w) && w != -1)
        height += (sep==-1 ? fnt->height : sep), width = 0, tw = 0;
    }else{
        fontglyph &g = fnt->glyphs[(unsigned char)(str[i] - fnt->glyphstart) % fnt->glyphcount];
        width += g.xs;
    }
  }
  return height;
}

