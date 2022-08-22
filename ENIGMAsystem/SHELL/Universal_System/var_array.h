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

#ifndef ENIGMA_VAR_ARRAY_H
#define ENIGMA_VAR_ARRAY_H

#include "var4.h"

namespace enigma_user {
var array_create(size_t size, evariant value=0);
var array_create_2d(size_t length, size_t height, evariant value=0);
bool array_equals(const var& arr1, const var& arr2);
void array_copy(var& dest, size_t dest_index, const var& src, size_t src_index, size_t length);
int array_length_1d(const var& v);
int array_length_2d(const var& v, int n);
int array_height_2d(const var& v);
void array_set(var& v, int pos, evariant value);
bool is_array(const var& v);
}  //namespace enigma_user

#endif  //ENIGMA_VAR_ARRAY_H
