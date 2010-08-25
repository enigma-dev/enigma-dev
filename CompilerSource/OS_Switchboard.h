// Identify the operating system
#define OS_WINDOWS 0
#define OS_LINUX   1
#define OS_MACOSX  2
#define OS_IPHONE  3
#define OS_ANDROID 4

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_WINDOWS"
  #define TARGET_PLATFORM_ID   OS_WINDOWS
  #define TARGET_PLATFORM_GRAPHICS "ENIGMA_GS_OPENGL"
#elif defined(linux) || defined(__linux) || defined(__linux__)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_XLIB"
  #define TARGET_PLATFORM_ID   OS_LINUX
  #define TARGET_PLATFORM_GRAPHICS "ENIGMA_GS_OPENGL"
#elif defined(__APPLE__) || defined(MACOSX) || defined(__MACH__) || defined(macintosh) || defined(Macintosh)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_COCOA" //ENIGMA_WS_IPHONE" ENIGMA_WS_COCOA
  #define TARGET_PLATFORM_ID   OS_MACOSX //OS_IPHONE OS_MACOSX
  #define TARGET_PLATFORM_GRAPHICS "ENIGMA_GS_OPENGL" // ENIGMA_GS_OPENGLES
  #define IPHONE_DEVICE 0
#elif defined(unix) || defined(__unix) || defined(__unix__)
  #define TARGET_PLATFORM_NAME "ENIGMA_WS_XLIB"
  #define TARGET_PLATFORM_ID   OS_MACOSX
  #define TARGET_PLATFORM_GRAPHICS "ENIGMA_GS_OPENGL"
#else
  #error Unable to determine name of the target platform.
#endif
