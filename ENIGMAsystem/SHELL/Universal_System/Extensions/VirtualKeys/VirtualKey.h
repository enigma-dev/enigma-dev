/** Copyright (C) 2019 Robert B. Colton
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

#ifndef ENIGMA_VIRTUAL_KEY_H
#define ENIGMA_VIRTUAL_KEY_H

#include "Universal_System/Resources/AssetArray.h"

using enigma::AssetArray;

namespace {

class VirtualKey {
  bool destroyed;
 public:
  bool visible, pressed;
  int x, y, width, height, keycode, sprite;

  VirtualKey() : destroyed(false), visible(false), pressed(false), x(0), y(0),
                 width(0), height(0), keycode(-1), sprite(-1) {}

  void destroy() { destroyed = true; }
  bool isDestroyed() const { return destroyed; }

  static const char* getAssetTypeName() { return "virtual key"; }
};

extern AssetArray<VirtualKey> virtual_keys;

} // namespace anonymous

#endif // ENIGMA_VIRTUAL_KEY_H
