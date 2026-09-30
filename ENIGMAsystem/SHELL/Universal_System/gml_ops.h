/** This file is a part of the ENIGMA Development Environment.
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

// Number semantics of GML operators. Generated GML code calls these instead of
// the C++ operators; GM8 compliance switches to GM8's rules.

#ifndef ENIGMA_GML_OPS_H
#define ENIGMA_GML_OPS_H

#include "compliance.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <utility>

namespace enigma {

// Round half to even, independent of the floating-point rounding mode. GM8
// (Delphi's Round) uses this for round(), array indices, views and integer
// conversions.
inline double round_half_even(double x) {
  double r = std::floor(x + 0.5);
  if (r - x == 0.5 && std::fmod(r, 2.0) != 0) r -= 1;
  return r;
}

// GM8 view variables hold integers; `v` is the result of an assignment.
template <class T> T &&gml_round_view(T &&v) {
  v = round_half_even((double)v);
  return std::forward<T>(v);
}

// background_width/height follow background_index.
void sync_background_slots();
template <class T> T &&gml_background_index_assigned(T &&v) {
  sync_background_slots();
  return std::forward<T>(v);
}

// GML round() rounds half to even (round(0.5) is 0, round(2.5) is 2), in GM8 and GMS alike.
inline double gml_round(double x) { return round_half_even(x); }

// Real to integer without the undefined cast: NaN is 0, and values past the
// type's range (infinities too) clamp to its ends.
template <class I> inline I saturate(double x) {
  if (x != x) return 0;
  if (x >= (double) std::numeric_limits<I>::max()) return std::numeric_limits<I>::max();
  if (x <= (double) std::numeric_limits<I>::min()) return std::numeric_limits<I>::min();
  return (I) x;
}

// Array index: GM8 rounds it half to even (a[1.5] is a[2]); standard truncates.
inline int gml_index(double x) { return saturate<int>(gm8_compliance() ? round_half_even(x) : x); }

// Real to integer where GML needs one (bitwise operands): GM8 rounds, the
// standard mode truncates.
inline int64_t gml_to_int(double x) {
  return saturate<int64_t>(gm8_compliance() ? round_half_even(x) : x);
}

inline double gml_bitand(double a, double b) { return (double) (gml_to_int(a) & gml_to_int(b)); }
inline double gml_bitor(double a, double b)  { return (double) (gml_to_int(a) | gml_to_int(b)); }
inline double gml_bitxor(double a, double b) { return (double) (gml_to_int(a) ^ gml_to_int(b)); }
inline double gml_bitnot(double a)           { return (double) ~gml_to_int(a); }
inline double gml_shl(double a, double b) {
  return (double) (int64_t) ((uint64_t) gml_to_int(a) << (gml_to_int(b) & 63));
}
// GM8 shifts right logically: -8 >> 1 is 2^63 - 4.
inline double gml_shr(double a, double b) {
  const int64_t v = gml_to_int(a), n = gml_to_int(b) & 63;
  return gm8_compliance() ? (double) ((uint64_t) v >> n) : (double) (v >> n);
}

// GM8 treats a real as true when it is at least 0.5.
template<typename T> inline bool gml_truth(const T &x) { return (double) x >= 0.5; }
inline bool gml_truth(bool x) { return x; }

// GM8 computes both operands of && || ^^, left first, even when the left one
// decides. A braced list evaluates in order.
struct gml_both {
  bool l, r;
  bool all() const { return l && r; }
  bool any() const { return l || r; }
  bool one() const { return l != r; }
};

}  // namespace enigma

#endif  // ENIGMA_GML_OPS_H
