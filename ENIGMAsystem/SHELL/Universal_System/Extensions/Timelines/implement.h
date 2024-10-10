namespace enigma {
  struct extension_timelines
  {
    int timeline_index;
    int timeline_speed;
    int timeline_position;
    extension_timelines(): timeline_index(-1), timeline_speed(1), timeline_position(0) {}
  };
}
