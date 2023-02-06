cmake_minimum_required(VERSION 3.14)

set(ENGINE_LIB "ENGINE")
set(ENGINE_LIB_DESCRIPTION "Enigma Engine")

project(${ENGINE_LIB} DESCRIPTION ${ENGINE_LIB_DESCRIPTION})

set(GENERAL_DIR "General")

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
  set(AUDIO "OpenAL")
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

add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/Alarms/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/Asynchronous/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/BasicGUI/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/Box2dPhysics/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/BulletDynamics/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/DataStructures/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/DateTime/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/DirectShow/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/ExternalFuncs/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/FileDropper/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/GM5Compat/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/GME/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/GTest/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/IniFilesystem/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/Json/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/libpng/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/MediaControlInterface/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/MotionPlanning/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/ParticleSystems/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/Paths/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/RegistrySpoof/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/StudioPhysics/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/Timelines/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/ttf/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/VirtualKeys/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/WindowsTouch/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/XInput/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/XRandR/)
add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System/Extensions/XTEST/)

add_subdirectory(${CMAKE_SOURCE_DIR}/Widget_Systems/${WIDGETS})
