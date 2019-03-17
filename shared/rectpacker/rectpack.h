/** Copyright (C) 2010-2011 Josh Ventura
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

#ifndef ENIGMA_RECT_PACT_H
#define ENIGMA_RECT_PACT_H

namespace enigma {

namespace rect_packer {
struct pvrect {
  int x, y, w, h, placed;
  pvrect();
  pvrect(int a, int b, int c, int d, int e);
};

struct rectpnode {
  rectpnode* child[2];
  int x, y, wid, hgt;
  int c;
  rectpnode();
  rectpnode(int xx, int yy, int w, int h, rectpnode* c1 = nullptr, rectpnode* c2 = nullptr);
  ~rectpnode();
  void rect(int xx, int yy, int w, int h);
};

void rncopy(rectpnode* h, pvrect* boxes, unsigned int c);
rectpnode* rninsert(rectpnode* who, unsigned int c, pvrect* boxes);
rectpnode* expand(rectpnode* who, int w, int h);
}  //namespace rect_packer

}  //namespace enigma

#endif  //ENIGMA_RECT_PACT_H
