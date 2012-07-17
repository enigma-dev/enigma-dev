#ifndef PATH_EXT_SET
#define PATH_EXT_SET
#endif

namespace enigma {
  struct extension_path
  {
    int path_index;
    int path_endaction;
    bool path_orientation;
    double path_position;
    double path_positionprevious;
    double path_scale;
    double path_speed;
    extension_path(): path_index(-1), path_endaction(0), path_orientation(0), path_position(0), path_positionprevious(0), path_scale(1), path_speed(0) {}
  };
}
