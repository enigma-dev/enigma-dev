#if defined(__APPLE__)

// ASan stub functions for macOS shared library compatibility
// When shared libraries use -Wl,-undefined,dynamic_lookup, ASan annotation
// functions are not resolved at build time. This file provides stubs that
// forward to the real ASan runtime functions loaded by the executable.

#include <dlfcn.h>
#include <string>

// Forward declaration for MESSAGE_TYPE enum
enum MESSAGE_TYPE : int {
  M_INFO = 0,
  M_WARNING = 1,
  M_ERROR = 2,
  M_FATAL_ERROR = 3,
  M_USER_ERROR = 4,
  M_FATAL_USER_ERROR = 5
};

namespace enigma_user {
  // Stub for show_debug_message used by tests
  void show_debug_message(std::string msg, MESSAGE_TYPE type = M_INFO) {
    // No-op stub for testing
  }
}

extern "C" {

// Stub for __sanitizer_annotate_double_ended_contiguous_container
void __sanitizer_annotate_double_ended_contiguous_container(
    const void *beg, const void *end,
    const void *old_mid, const void *new_mid,
    const void *old_end, const void *new_end) {
  // Look up the real function from the ASan runtime
  typedef void (*annotate_fn)(const void*, const void*, const void*, 
                               const void*, const void*, const void*);
  static annotate_fn real_fn = nullptr;
  
  if (!real_fn) {
    real_fn = (annotate_fn)dlsym(RTLD_DEFAULT, 
        "___sanitizer_annotate_double_ended_contiguous_container");
  }
  
  if (real_fn) {
    real_fn(beg, end, old_mid, new_mid, old_end, new_end);
  }
}

} // extern "C"

#endif  // __APPLE__
