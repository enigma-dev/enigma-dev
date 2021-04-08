/** Copyright (C) 2008 Josh Ventura
/**
*** Nabeel Danish 
**/

#ifndef ENIGMA_POLYGON_H
#define ENIGMA_POLYGON_H

#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"
#include <string>

namespace enigma_user {
	int polygon_add(int height, int width);
	int polygon_add_point(int id, int x, int y);
}

#endif  // !ENIGMA_POLYGON_H