/** Copyright (C) 2021 Nabeel Danish
***
***/

#include "polygon_internal.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include <string>

using enigma::Polygon;
using enigma::polygons;

namespace enigma_user {
	int polygon_add(int height, int width) { ;
		Polygon polygon(height, width, 0, 0);
		int id = polygons.add(std::move(polygon));
		return id;
	}
	int polygon_add_point(int id, int x, int y) {
		polygons.get(id).addPoint(x, y);
		return id;
	}
	int polygon_get_width(int polyid) {
		return polygons.get(polyid).getWidth();
	}
	int polygon_get_height(int polyid) {
		return polygons.get(polyid).getHeight();
	}
	int polygon_get_xoffset(int id) {
		return polygons.get(id).getXOffset();
	}
	int polygon_get_yoffset(int id) {
		return polygons.get(id).getYOffset();
	}
	bool polygon_exists(int id) {
		return polygons.exists(id);
	}
	void polygon_decompose(int id) {
		// polygons.get(id).decomposeConcave();
	}
	void polygon_set_scale(int id, double scale) {
		polygons.get(id).setScale(scale);
	}
	void polygon_set_angle(int id, double angle) {
		polygons.get(id).setAngle(angle);
	}
}
