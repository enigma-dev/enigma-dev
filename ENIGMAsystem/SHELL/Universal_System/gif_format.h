/*
 * Copyright (C) 2014 Seth N. Hetu
 *
 * This file is a part of the ENIGMA Development Environment.
 *
 * ENIGMA is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the license or any later version.
 *
 * This application and its source code is distributed AS-IS, WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this code. If not, see <http://www.gnu.org/licenses/>
 */

/*
 * This file defines functions for working with the GIF format (currently only loading).
 * Since GIF is a somewhat polarizing format, here's a few salient points:
 *  1) If someone tells you that "GIF is patented", they are 100% wrong. Politely point
 *     them at the standard; it's just a wrapper format: (http://www.w3.org/Graphics/GIF/spec-gif89a.txt)
 *  2) If someone tells you taht "GIF uses LZW, which is patented", they are correct, but
 *     their information is out-dated. All relevant LZW patents have now expired.
 */

#ifndef ENIGMA_GIFFORMAT_H
#define ENIGMA_GIFFORMAT_H

namespace enigma
{
unsigned int load_gif_file(const char* filename, unsigned char*& out, unsigned int& image_width, unsigned int& image_height, int& num_images);
const char* load_gif_error_text(unsigned int err);
} //namespace enigma

#endif //ENIGMA_GIFFORMAT_H
