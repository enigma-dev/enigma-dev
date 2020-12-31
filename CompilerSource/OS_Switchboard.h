// Identify the operating system
// Note: You should NOT ADD ON to this list if LateralGM doesn't run on the platform you intend to add!
#define OS_WINDOWS 0
#define OS_LINUX   1
#define OS_MACOSX  2
#define OS_BSD     3

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #define CURRENT_PLATFORM_NAME "Windows"
  #define CURRENT_PLATFORM_ID   OS_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__)
  #define CURRENT_PLATFORM_NAME "Linux"
  #define CURRENT_PLATFORM_ID   OS_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(__MACH__) || defined(macintosh) || defined(Macintosh)
  #define CURRENT_PLATFORM_NAME "MacOSX"
  #define CURRENT_PLATFORM_ID   OS_MACOSX
  #define IPHONE_DEVICE 0
  //#define WRITE_UNIMPLEMENTED_TXT 1
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
  #define CURRENT_PLATFORM_NAME "BSD"
  #define CURRENT_PLATFORM_ID   OS_BSD
#elif defined(unix) || defined(__unix) || defined(__unix__)
  #define CURRENT_PLATFORM_NAME "Linux"
  #define CURRENT_PLATFORM_ID   OS_LINUX
#else
  #error Unable to determine name of the target platform.
#endif
