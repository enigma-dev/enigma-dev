// Identify the operating system
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_WINDOWS"
#elif defined(linux) || defined(__linux) || defined(__linux__)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_XLIB"
#elif defined(__APPLE__) || defined(MACOSX) || defined(__MACH__) || defined(macintosh) || defined(Macintosh)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_COCOA"
#elif defined(unix) || defined(__unix) || defined(__unix__)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_XLIB"
#else
  #error Unable to determine name of the target platform.
#endif
