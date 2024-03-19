set(XLIB_PLATFORM_LIB "xlib_platform_lib")
set(XLIB_PLATFORM_LIB_DESCRIPTION "XLib Plaform")

project(${XLIB_PLATFORM_LIB} DESCRIPTION ${XLIB_PLATFORM_LIB_DESCRIPTION})

if (OS MATCHES LINUX-PLATFORM)
    file(GLOB_RECURSE XLIB_PLATFORM_LIB_SOURCES 
        ../General/POSIX/Linux/*.cpp 
        ../xlib/Linux/*.cpp)
elseif(OS MATCHES FREEBSD-PLATFORM)
    file(GLOB_RECURSE XLIB_PLATFORM_LIB_SOURCES 
        ../General/POSIX/BSD/*.cpp
        ../xlib/BSD/*.cpp)
elseif(OS MATCHES DRAGONFLY-PLATFORM)
    file(GLOB_RECURSE XLIB_PLATFORM_LIB_SOURCES 
        ../General/POSIX/BSD/*.cpp 
        ../xlib/BSD/*.cpp)
endif()

file(GLOB_RECURSE XLIB_PLATFORM_LIB_SOURCES 
    *.cpp 
    ../General/POSIX/*.cpp
    ../General/PFshell/PFshell.cpp)

#message("Platform/xlib Sources: ${XLIB_PLATFORM_LIB_SOURCES}")

add_library(${XLIB_PLATFORM_LIB} ${XLIB_PLATFORM_LIB_SOURCES})
