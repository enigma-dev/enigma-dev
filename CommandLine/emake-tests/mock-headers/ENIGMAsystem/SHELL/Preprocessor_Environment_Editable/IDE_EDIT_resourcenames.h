// Mock header for testing - minimal version
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-declarations"

namespace enigma {
  const char *resource_file_path = "";
}

namespace enigma_user {
  // Empty enums for resource types (tests don't need actual resources)
  enum {  // object names
  };
  std::string object_get_name(int i) { return "<undefined>"; }

  enum {  // sprite names
  };
  std::string sprite_get_name(int i) { return "<undefined>"; }

  enum {  // background names
  };
  std::string background_get_name(int i) { return "<undefined>"; }

  enum {  // font names
  };
  std::string font_get_name(int i) { return "<undefined>"; }

  enum {  // timeline names
  };
  std::string timeline_get_name(int i) { return "<undefined>"; }

  enum {  // path names
  };
  std::string path_get_name(int i) { return "<undefined>"; }

  enum {  // sound names
  };
  std::string sound_get_name(int i) { return "<undefined>"; }

  enum {  // script names
  };
  std::string script_get_name(int i) { return "<undefined>"; }

  enum {  // shader names
  };
  std::string shader_get_name(int i) { return "<undefined>"; }
}

#pragma clang diagnostic pop
