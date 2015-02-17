//#include "structs.h"

namespace enigma_user
{
  int touchscreen_init(int cam);
  int touchscreen_update();
  int touchscreen_free();
  int blob_count();
  int blob_x(int i);
  int blob_y(int i);
  int blob_area(int i);
  int blob_id(int i);
  bool blob_pressed(int id);
  bool blob_released(int id);

  int blob_get_bbox_l(int i);
  int blob_get_bbox_t(int i);
  int blob_get_bbox_r(int i);
  int blob_get_bbox_b(int i);

  bool blob_get_pixel(int i, int px, int py);

  int blob_x_id(int i);
  int blob_y_id(int i);
  bool blob_pressed_id(int id);
  bool blob_released_id(int id);

  void touchscreen_to_surface(int surf, int step, int tresh);
}
