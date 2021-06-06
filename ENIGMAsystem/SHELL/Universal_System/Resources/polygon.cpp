/** Copyright (C) 2021 Nabeel Danish
***
***/

#include "polygon_internal.h"
#include <string>

using enigma::Polygon;
using enigma::Vector2D;
using enigma::polygons;

namespace enigma_user {
	int polygon_add(int height, int width) { 
		printf("Height = %d, Width = %d\n", height, width);
		Polygon polygon(height, width, 0, 0);
		int id = polygons.add(std::move(polygon));
		printf("id = %d\n", id);
		return id;
	}
	int polygon_add_point(int id, int x, int y) {
		polygons.get(id).addPoint(x, y);
		return id;
	}
}
