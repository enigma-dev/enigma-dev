/*
 * Copyright (C) 2014 Robert B. Colton
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_SUB_GLYPHRANGE_H
#define ENIGMA_SUB_GLYPHRANGE_H

#include "backend/sub/Glyph.h"

namespace deprecated {
namespace JavaStruct {

struct GlyphRange
{
	int rangeMin;
	int rangeMax;
	Glyph *glyphs;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
