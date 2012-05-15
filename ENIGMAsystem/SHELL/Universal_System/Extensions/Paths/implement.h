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
  };
}
