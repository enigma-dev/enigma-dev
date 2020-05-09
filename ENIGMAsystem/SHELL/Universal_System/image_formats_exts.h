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
#include <filesystem>

namespace enigma
{

using ImageLoadFunction = std::function<std::vector<RawImage>(const std::filesystem::path&)>;
using ImageSaveFunction = std::function<int(const std::filesystem::path&, const unsigned char*, unsigned, unsigned, unsigned, unsigned, bool)>;

void image_add_loader(const std::filesystem::path&, ImageLoadFunction fnc);
void image_add_saver(const std::filesystem::path&, ImageSaveFunction fnc);

} //namespace enigma

#endif //ENIGMA_IMAGEFORMATS_EXTS_H
