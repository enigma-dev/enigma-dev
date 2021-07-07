// argument0 = shape type
//   0 = d3d_draw_block
//   1 = d3d_draw_cylinder
//   2 = d3d_draw_cone
//   3 = d3d_draw_ellipsoid
//   4 = d3d_draw_wall
//   5 = d3d_draw_floor
// argument1 = size
// argument2 = texture id
var shape = argument0,
	size = argument1,
	tex = argument2;

switch (shape) {
case 0:
	d3d_draw_block(
		-size, -size, -size,
		size, size, size,
		tex, 1, 1
	);
	return 0;
case 1:
	d3d_draw_cylinder(
		-size, -size, -size,
		size, size, size,
		tex, 1, 1,
		true, 8
	);
	return 0;
case 2:
	d3d_draw_cone(
		-size, -size, -size,
		size, size, size,
		tex, 1, 1,
		true, 8
	);
	return 0;
case 3:
	d3d_draw_ellipsoid(
		-size, -size, -size,
		size, size, size,
		tex, 1, 1, 8
	);
	return 0;
case 4:
	d3d_draw_wall(
		-size, 0, -size,
		size, 0, size,
		tex, 1, 1
	);
	return 0;
case 5:
	d3d_draw_floor(
		-size, -size, 0,
		size, size, 0,
		tex, 1, 1
	);
	return 0;
}
