cmake_minimum_required(VERSION 3.14)

set(ENGINE_LIB "ENGINE")
set(ENGINE_LIB_DESCRIPTION "Enigma Engine")

project(${ENGINE_LIB} DESCRIPTION ${ENGINE_LIB_DESCRIPTION})

set(LINUX-PLATFORM "Linux")
set(FREEBSD-PLATFORM "FreeBSD")
set(ANDROID-PLATFORM "Android")
set(WINDOWS-PLATFORM "Windows")
set(MACOSX-PLATFORM "MacOSX")
set(DRAGONFLY-PLATFORM "DragonFly")

set(GENERAL_DIR "General")

set(ENIGMA_ROOT "../../")

# GMODE { Run, Build, Debug, Compile }
if(NOT DEFINED GMODE)
  set(GMODE "Run")
endif()

# GRAPHICS { Graphics_Systems/* }
if(NOT DEFINED GRAPHICS)
  set(GRAPHICS "OpenGL1")
endif()

message("Graphics Sys: ${GRAPHICS}")

# AUDIO { Audio_Systems/* }
if(NOT DEFINED AUDIO)
  set(AUDIO "None") # OpenAL
endif()

message("Audio Sys: ${AUDIO}")

# COLLISION { Collision_Systems/* }
if(NOT DEFINED COLLISION)
  set(COLLISION "BBox")
endif()

message("Collision Sys: ${COLLISION}")

# WIDGETS { Widget_Systems/* }
if(NOT DEFINED WIDGETS)
  set(WIDGETS "None")
endif()

message("Widgets Sys: ${WIDGETS}")

# NETWORKING { Networking_Systems/* }
if(NOT DEFINED NETWORKING)
  set(NETWORKING "None")
endif()

message("Networking Sys: ${NETWORKING}")

add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/${AUDIO})

add_subdirectory(${CMAKE_SOURCE_DIR}/Collision_Systems/${COLLISION})

add_subdirectory(${CMAKE_SOURCE_DIR}/Networking_Systems/${NETWORKING})

add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System)

add_subdirectory(${CMAKE_SOURCE_DIR}/Widget_Systems/${WIDGETS})

add_subdirectory(${CMAKE_SOURCE_DIR}/Platforms/SDL)
add_subdirectory(${CMAKE_SOURCE_DIR}/Platforms/Win32)
add_subdirectory(${CMAKE_SOURCE_DIR}/Platforms/xlib)
