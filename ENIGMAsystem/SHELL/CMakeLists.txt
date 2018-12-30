cmake_minimum_required(VERSION 3.10)

set(EXE "game")
set(EXE_VERSION "0.1")
set(EXE_DESCRIPTION "ENIGMA Game")

project("${EXE}")

# Extra library searches
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmakemodules")

# ENIGMA's root
set(ENIGMA_DIR "../..")

include_directories(. "${CODEGEN}" "${ENIGMA_DIR}/shared/lodepng/")

if(WIN32)
  add_executable("${EXE}" WIN32 SHELLmain.cpp)
else()
  add_executable("${EXE}" SHELLmain.cpp)
endif(WIN32)

# Platform
add_subdirectory("Platforms/General/")
target_link_libraries("${EXE}" "PLATFORM_GENERAL")

if (NOT "${PLATFORM}" STREQUAL "Win32")
  add_subdirectory("Platforms/General/POSIX")
  target_link_libraries("${EXE}" "PLATFORM_POSIX")
endif()

if (("${PLATFORM}" STREQUAL "SDL") AND ("${TARGET-PLATFORM}" STREQUAL "Windows"))
  add_subdirectory("Bridges/SDL-Win32/")
  target_link_libraries("${EXE}" "SDL-WIN32-Bridge")
endif()

add_subdirectory("Platforms/${PLATFORM}/")
target_link_libraries("${EXE}" "PLATFORM_${PLATFORM}")

# Graphics
add_subdirectory("Graphics_Systems/General/")
target_link_libraries("${EXE}" "GRAPHICS_GENERAL")

# Convoluted bridges logic
if ("${PLATFORM}" STREQUAL "Win32")
  if ("${GRAPHICS}" STREQUAL "Direct3D9")
    add_subdirectory("Bridges/Win32-Direct3D9")
    target_link_libraries("${EXE}" "Win32-Direct3D9-Bridge")
  elseif ("${GRAPHICS}" STREQUAL "Direct3D11")
    add_subdirectory("Bridges/Win32-Direct3D9")
    target_link_libraries("${EXE}" "Win32-Direct3D11-Bridge")
  endif()
endif()

if ("${PLATFORM}" STREQUAL "SDL")
  if ("${GRAPHICS}" STREQUAL "Direct3D9")
    add_subdirectory("Bridges/Win32-Direct3D9")
    target_link_libraries("${EXE}" "Win32-Direct3D9-Bridge")
    add_subdirectory("Bridges/SDL-Direct3D/")
    target_link_libraries("${EXE}" "SDL-Direct3D-Bridge")
  elseif ("${GRAPHICS}" STREQUAL "Direct3D11")
    add_subdirectory("Bridges/Win32-Direct3D9")
    target_link_libraries("${EXE}" "Win32-Direct3D11-Bridge")
    add_subdirectory("Bridges/SDL-Direct3D/")
    target_link_libraries("${EXE}" "SDL-Direct3D-Bridge")
  endif()
endif()

# OpenGL bridges seems to almost follow a pattern unlike directx
string(FIND "${GRAPHICS}" "OpenGL" containsGL)
if (NOT ${containsGL} EQUAL -1)
  add_subdirectory("Bridges/${PLATFORM}-OpenGL")
  target_link_libraries("${EXE}" "${PLATFORM}-OpenGL-Bridge")
  if ("${PLATFORM}" STREQUAL "Win32")
    add_subdirectory("Bridges/${PLATFORM}-${GRAPHICS}")
    target_link_libraries("${EXE}" "${PLATFORM}-${GRAPHICS}-Bridge")
  endif()
endif()

# Universal System //FIXME: circular dependency between graphics system and universal
add_subdirectory("Universal_System/")
target_link_libraries("${EXE}" "UNIVERSAL_SYSTEM")

add_subdirectory("Graphics_Systems/${GRAPHICS}/")
target_link_libraries("${EXE}" "GRAPHICS_${GRAPHICS}")

target_link_libraries("${EXE}" "UNIVERSAL_SYSTEM") #//Remove when fixed

# Audio
add_subdirectory("Audio_Systems/${AUDIO}/")
target_link_libraries("${EXE}" "AUDIO_${AUDIO}")

# Collisions
add_subdirectory("Collision_Systems/${COLLISION}/")
target_link_libraries("${EXE}" "COLLISION_${COLLISION}")

# Widgets
add_subdirectory("Widget_Systems/${WIDGETS}/")
target_link_libraries("${EXE}" "WIDGETS_${WIDGETS}")

# Shared Sources
add_subdirectory("${ENIGMA_DIR}/shared/lodepng" lodepng)
target_link_libraries("${EXE}" "lodepng")

# zlib
find_package(ZLIB REQUIRED)
target_link_libraries("${EXE}" "${ZLIB_LIBRARIES}")
