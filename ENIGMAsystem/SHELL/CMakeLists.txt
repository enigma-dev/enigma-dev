cmake_minimum_required(VERSION 3.14)

set(ENGINE_LIB "ENGINE")
set(ENGINE_LIB_DESCRIPTION "Enigma Engine")

project(${ENGINE_LIB} DESCRIPTION ${ENGINE_LIB_DESCRIPTION})

# GMODE { Run, Build, Debug, Compile }
if(GMODE STREQUAL "")
    set(GMODE Run)
endif()

# GRAPHICS { Graphics_Systems/* }
if(GRAPHICS STREQUAL "")
    set(GRAPHICS OpenGL1)
endif()

# AUDIO { Audio_Systems/* }
if(AUDIO STREQUAL "")
    set(AUDIO "OpenAL")
endif()

# COLLISION { Collision_Systems/* }
if(COLLISION STREQUAL "")
    set(COLLISION BBox)
endif()

# WIDGETS { Widget_Systems/* }
if(WIDGETS STREQUAL "")
    set(WIDGETS None)
endif()

# NETWORKING { Networking_Systems/* }
if(NETWORKING STREQUAL "")
    set(NETWORKING None)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/OpenAL)
add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/DirectSound)
add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/None)
add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/SDL)
add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/XAudio2)

add_subdirectory(${CMAKE_SOURCE_DIR}/Collision_Systems/BBox)
add_subdirectory(${CMAKE_SOURCE_DIR}/Collision_Systems/General)
add_subdirectory(${CMAKE_SOURCE_DIR}/Collision_Systems/None)
add_subdirectory(${CMAKE_SOURCE_DIR}/Collision_Systems/Polygon)
add_subdirectory(${CMAKE_SOURCE_DIR}/Collision_Systems/Precise)

# add_subdirectory(${CMAKE_SOURCE_DIR}/Networking_Systems/Asynchronous)
add_subdirectory(${CMAKE_SOURCE_DIR}/Networking_Systems/BerkeleySockets)
add_subdirectory(${CMAKE_SOURCE_DIR}/Networking_Systems/DirectPlay)
# add_subdirectory(${CMAKE_SOURCE_DIR}/Networking_Systems/None)

add_subdirectory(${CMAKE_SOURCE_DIR}/Universal_System)
