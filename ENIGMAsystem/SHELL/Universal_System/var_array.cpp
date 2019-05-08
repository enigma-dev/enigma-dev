/********************************************************************************\
**                                                                              **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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

#include "var_array.h"
#include "var4.h"

namespace enigma_user {

var array_create(size_t size, variant value) { return var(value, size); }
var array_create_2d(size_t length, size_t height, variant value) { return var(value, length, height); }
bool array_equals(const var& arr1, const var& arr2) {
  if (arr1.array_len() != arr2.array_len()) return false;
  for (int i = 0; i < arr1.array_len(); i++) {
    if (arr1[i] != arr2[i]) return false;
  }
  return true;
}
void array_copy(var& dest, size_t dest_index, const var& src, size_t src_index, size_t length) {
  for (size_t i = 0; i < length; i++) {
    dest[dest_index + i] = src[src_index + i];
  }
}
int array_length_1d(const var& v) { return v.array_len(); }
int array_length_2d(const var& v, int n) { return v.array_len(n); }
int array_height_2d(const var& v) { return v.array_height(); }
bool is_array(const var& v) {
  //There is no way (currently) to downsize an array from >1 element, so this might not be accurate.
  return (v.array_height() > 1) || (v.array_len() > 1);
}

}  // namespace enigma_user
