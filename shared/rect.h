#ifndef ENIGMA_RECT_H
#define ENIGMA_RECT_H

namespace enigma {

template <class T = int>
struct Rect {
  Rect() {}
  Rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
  T x = 0;
  T y = 0;
  T w = 0;
  T h = 0;
  
  // Aliases to appease existing code
  T left() const { return x; }
  T top() const { return y; }
  T right() const { return x + w; }
  T bottom() const { return y + h; }
};

}

#endif
