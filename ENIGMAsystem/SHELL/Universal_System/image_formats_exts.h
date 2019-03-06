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

#ifndef ENIGMA_IMAGEFORMATS_EXTS_H
#define ENIGMA_IMAGEFORMATS_EXTS_H

#include <functional>
#include <string>

namespace enigma
{

using ImageLoadFunction = std::function<unsigned char*(std::string, unsigned int*, unsigned int*, unsigned int*, unsigned int*, bool)>;
using ImageSaveFunction = std::function<int(std::string, const unsigned char*, unsigned, unsigned, unsigned, unsigned, bool)>;

void image_add_loader(std::string format, ImageLoadFunction fnc);
void image_add_saver(std::string format, ImageSaveFunction fnc);

} //namespace enigma

#endif //ENIGMA_IMAGEFORMATS_EXTS_H
