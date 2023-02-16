set(COCOA_PLATFORM_LIB "cocoa_platform_lib")
set(COCOA_PLATFORM_LIB_DESCRIPTION "Cocoa Plaform")

project(${COCOA_PLATFORM_LIB} DESCRIPTION ${COCOA_PLATFORM_LIB_DESCRIPTION})

file(GLOB_RECURSE COCOA_PLATFORM_LIB_SOURCES 
    *.cpp 
    ../General/POSIXthreads.cpp
    ../General/UNIXfilemanip.cpp
    ../General/POSIX/POSIXdirectory.cpp
    ../General/PFshell/PFshell.cpp
    *.m)

add_library(${COCOA_PLATFORM_LIB} ${COCOA_PLATFORM_LIB_SOURCES})
