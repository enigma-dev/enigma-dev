#ifndef PATH_EXT_SET
#define PATH_EXT_SET
#endif

namespace enigma {
  struct extension_path
  {
    int path_index;
    int path_endaction;
    bool path_orientation;
    cs_scalar path_position;
    cs_scalar path_positionprevious;
    cs_scalar path_scale;
    cs_scalar path_speed;
    
    cs_scalar path_xstart;
    cs_scalar path_ystart;
    
    extension_path(): path_index(-1), path_endaction(0), path_orientation(0), path_position(0), path_positionprevious(0), path_scale(1), path_speed(0) {}

    virtual evariant myevent_pathend() { return 1; }
  };
}
