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

#include <vector>
#include <algorithm>

namespace enigma {

namespace rect_packer {

struct pvrect {
  pvrect(int x = 0, int y = 0, unsigned w = 0, unsigned h = 0, int placed = -1) :  
    x(x), y(y), w(w), h(h), placed(placed) {}
  int x, y;
  unsigned w, h;
  int placed;
  unsigned area() const { return w * h; };
};

bool pack_rectangles(std::vector<pvrect>& rects, unsigned& width, unsigned &height);

template <class T>
bool pack_rectangles(std::vector<T>& vec, unsigned& width, unsigned &height) {

  // sort our vector by area
  std::sort(vec.begin(), vec.end(), [](const T& a, const T& b) {
    return (a.dimensions.area() > b.dimensions.area());
  });

  std::vector<pvrect> copy(vec.size());
  size_t currIndex = 0;
  for (auto& i : vec) {
    copy[currIndex++] = i.dimensions;
  }

  pack_rectangles(copy, width, height);

  // Copy our updated positions back to our array
  currIndex = 0;
  for (pvrect& rect : copy) {
    vec[currIndex].dimensions.x = rect.x;
    vec[currIndex].dimensions.y = rect.y;
    currIndex++;
  }

  return true;
}


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
