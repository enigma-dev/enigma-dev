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

#include <math.h>
#include <string>
#include "OpenGLHeaders.h"
using namespace std;

#include "../../Universal_System/fontstruct.h"

namespace enigma {
  static int currentfont = -1;
  extern unsigned bound_texture;
}

using namespace enigma;
using namespace std;

void draw_text(int x,int y,string str)
{
  font *fnt = fontstructarray[currentfont];
  
  if (bound_texture != fnt->texture)
    glBindTexture(GL_TEXTURE_2D, bound_texture = fnt->texture);
  
  int xx = x, yy = y;
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
      glBegin(GL_QUADS);
        glTexCoord2f(g.tx,  g.ty);
          glVertex2i(xx + g.x,  yy + g.y);
        glTexCoord2f(g.tx2, g.ty);
          glVertex2i(xx + g.x2, yy + g.y);
        glTexCoord2f(g.tx2, g.ty2);
          glVertex2i(xx + g.x2, yy + g.y2);
        glTexCoord2f(g.tx,  g.ty2);
          glVertex2i(xx + g.x,  yy + g.y2);
      glEnd();
      xx += g.xs;
    }
  }
}

unsigned int font_get_texture(int fnt)
{
  font *f = fontstructarray[fnt];
  return f ? f->texture : unsigned(-1);
}

void draw_set_font(int fnt)
{
  enigma::currentfont = fnt;
}
