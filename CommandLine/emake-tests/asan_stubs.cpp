// ASan stub functions for macOS shared library compatibility
// When shared libraries use -Wl,-undefined,dynamic_lookup, ASan annotation
// functions are not resolved at build time. This file provides stubs that
// forward to the real ASan runtime functions loaded by the executable.

#include <dlfcn.h>

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
