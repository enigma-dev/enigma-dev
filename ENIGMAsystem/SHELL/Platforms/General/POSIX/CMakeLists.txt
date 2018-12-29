set(PLATFORM_POSIX_SOURCES
   main.cpp
   POSIXthreads.cpp
   POSIXworkdir.cpp
   POSIXproccess.cpp
   Utility.cpp
   POSIXfilemanip.cpp
)

add_library("PLATFORM_POSIX" "${PLATFORM_POSIX_SOURCES}")
