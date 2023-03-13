if(NOT DEFINED PKG-CONFIG)
  set(PKG-CONFIG "pkg-config")
endif()

set(SDL_PLATFORM_LIB "sdl_platform_lib")
set(SDL_PLATFORM_LIB_DESCRIPTION "SDL Plaform")

project(${SDL_PLATFORM_LIB} DESCRIPTION ${SDL_PLATFORM_LIB_DESCRIPTION})

file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
    *.cpp 
    ../General/POSIX/Utility.cpp)

if (TARGET-PLATFORM MATCHES ANDROID-PLATFORM)
    file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
        ../SDL/Android/*.cpp)
else()
    file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
        ../SDL/Desktop/Window.cpp)
endif()

if (TARGET-PLATFORM MATCHES WINDOWS-PLATFORM)
    file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
        ../Win32/WINDOWSfilemanip.cpp
        ../SDL/Win32/Window.cpp)
else()
    if (NOT TARGET-PLATFORM MATCHES ANDROID-PLATFORM)
        file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
            ../General/PFshell/PFshell.cpp
            ../General/POSIX/shell.cpp)
    endif()

    file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
        ../General/POSIX/POSIXfilemanip.cpp
        ../General/POSIX/POSIXfilemanip.cpp)
    
    if (TARGET-PLATFORM MATCHES MACOSX-PLATFORM)
        file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
            ../SDL/Cocoa/Window.cpp
            ../SDL/Cocoa/CocoaWindow.m)
    elseif(TARGET-PLATFORM MATCHES LINUX-PLATFORM)
        file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
            ../SDL/xlib/Window.cpp)
    elseif(TARGET-PLATFORM MATCHES FREEBSD-PLATFORM)
        file(GLOB_RECURSE SDL_PLATFORM_LIB_SOURCES 
            ../SDL/xlib/Window.cpp)
    endif()
endif()

#message("Platform/SDL Sources: ${SDL_PLATFORM_LIB_SOURCES}")

add_library(${SDL_PLATFORM_LIB} ${SDL_PLATFORM_LIB_SOURCES})
