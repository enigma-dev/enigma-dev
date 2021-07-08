/** Copyright (C) 2021 Nabeel Danish
/** 
**/

#ifndef ENIGMA_POLYGON_H
#define ENIGMA_POLYGON_H

#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"
#include <string>

namespace enigma_user {
	int polygon_add(int height, int width);
	int polygon_add_point(int id, int x, int y);
	int polygon_get_width(int id);
	int polygon_get_height(int id);
	int polygon_get_xoffset(int id);
	int polygon_get_yoffset(int id);
	bool polygon_exists(int id);
	void polygon_decompose(int id);
	void polygon_set_scale(int id, double scale);
	void polygon_set_angle(int id, double angle);
}

#endif  // !ENIGMA_POLYGON_H
