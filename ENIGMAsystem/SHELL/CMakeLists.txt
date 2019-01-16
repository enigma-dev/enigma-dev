cmake_minimum_required(VERSION 3.13)

# Remove this "if" when windows less buggy
if (WIN32)
  get_filename_component(EXE "${OUTPUTNAME}" NAME_WE)
else()
  get_filename_component(EXE "${OUTPUTNAME}" NAME)
endif (WIN32)

set(EXE_VERSION "0.1")
set(EXE_DESCRIPTION "ENIGMA Game")

project("${EXE}")

# Make codegen dir
file(MAKE_DIRECTORY "${CODEGEN}/Preprocessor_Environment_Editable/")

# Extra library searches
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmakemodules")

# ENIGMA's root
set(ENIGMA_DIR "../..")

include_directories(. "${CMAKE_CURRENT_BINARY_DIR}" "${ENIGMA_DIR}/shared/lodepng/")

if ("${GMODE}" STREQUAL "Debug")
  add_definitions(-DDEBUG_MODE)
endif()

set(PREPROS "${CODEGEN}/Preprocessor_Environment_Editable")
configure_file("${CODEGEN}/API_Switchboard.h" "API_Switchboard.h" COPYONLY)
configure_file("${PREPROS}/GAME_SETTINGS.h" "Preprocessor_Environment_Editable/GAME_SETTINGS.h" COPYONLY)
configure_file("${PREPROS}/LIBINCLUDE.h" "Preprocessor_Environment_Editable/LIBINCLUDE.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_objectfunctionality.h" "Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_events.h" "Preprocessor_Environment_Editable/IDE_EDIT_events.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_resourcenames.h" "Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_evparent.h" "Preprocessor_Environment_Editable/IDE_EDIT_evparent.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_evparent.h" "Preprocessor_Environment_Editable/IDE_EDIT_evparent.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_roomarrays.h" "Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_fontinfo.h" "Preprocessor_Environment_Editable/IDE_EDIT_fontinfo.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_roomcreates.h" "Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_globals.h" "Preprocessor_Environment_Editable/IDE_EDIT_globals.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_shaderarrays.h" "Preprocessor_Environment_Editable/IDE_EDIT_shaderarrays.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_inherited_locals.h" "Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_timelines.h" "Preprocessor_Environment_Editable/IDE_EDIT_timelines.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_modesenabled.h" "Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_whitespace.h" "Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_whitespace.h" "Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_object_switch.h" "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_objectaccess.h" "Preprocessor_Environment_Editable/IDE_EDIT_objectaccess.h" COPYONLY)
configure_file("${PREPROS}/IDE_EDIT_objectdeclarations.h" "Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h" COPYONLY)

if(WIN32)
  add_executable("${EXE}" WIN32 SHELLmain.cpp)
  if(MSVC)
    add_compile_options(/permissive-)
  endif(MSVC)
else()
  add_executable("${EXE}" SHELLmain.cpp)
endif(WIN32)

# Platform
if("${PLATFORM}" STREQUAL "Win32")
  add_definitions(-DENIGMA_PLATFORM_WINDOWS)
elseif("${PLATFORM}" STREQUAL "SDL")
  add_definitions(-DENIGMA_PLATFORM_SDL)
else()
  add_definitions(-DENIGMA_PLATFORM_UNKNOWN)
endif()
add_subdirectory("Platforms/${PLATFORM}/")
target_link_libraries("${EXE}" "PLATFORM_${PLATFORM}")

if (("${PLATFORM}" STREQUAL "SDL") AND ("${TARGET-PLATFORM}" STREQUAL "Windows"))
  add_subdirectory("Bridges/SDL-Win32/")
  target_link_libraries("${EXE}" "SDL-WIN32-Bridge")
endif()

add_subdirectory("Platforms/General/")
target_link_libraries("${EXE}" "PLATFORM_GENERAL")

if (NOT "${PLATFORM}" STREQUAL "Win32")
  add_subdirectory("Platforms/General/POSIX")
  target_link_libraries("${EXE}" "PLATFORM_POSIX")
endif()

# Graphics
find_package(GLM REQUIRED)
include_directories("${GLM_INCLUDE_DIRS}")

add_subdirectory("Graphics_Systems/General/")
target_link_libraries("${EXE}" "GRAPHICS_GENERAL")

# Convoluted bridges logic
if ("${PLATFORM}" STREQUAL "Win32")
  if ("${GRAPHICS}" STREQUAL "Direct3D9")
    add_subdirectory("Bridges/Win32-Direct3D9")
    target_link_libraries("${EXE}" "Win32-Direct3D9-Bridge")
  elseif ("${GRAPHICS}" STREQUAL "Direct3D11")
    add_subdirectory("Bridges/Win32-Direct3D11")
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

# Networking
if (NOT "${NETWORKING}" STREQUAL "None")
  add_subdirectory("Networking_Systems/${NETWORKING}/")
  target_link_libraries("${EXE}" "NETWORKING_${NETWORKING}")
endif()

if (NOT "${EXTENSIONS}" STREQUAL "")
  string(LENGTH "${EXTENSIONS}" STRLEN)
  string(SUBSTRING ${EXTENSIONS} 1 ${STRLEN} EXTENSIONS)
  string(REPLACE " " ";" EXTENSIONS_LIST ${EXTENSIONS})
  
  foreach(EXT ${EXTENSIONS_LIST})
    get_filename_component(EXTNAME "${EXT}" NAME)
    add_subdirectory("${EXT}/")
    target_link_libraries("${EXE}" "EXTENSION_${EXTNAME}")
  endforeach()
endif()

get_filename_component(OUTPUT_PATH "${OUTPUTNAME}" PATH)
install(TARGETS "${EXE}" DESTINATION "${OUTPUT_PATH}")
