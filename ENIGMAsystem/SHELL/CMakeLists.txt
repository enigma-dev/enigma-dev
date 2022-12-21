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
    set(AUDIO OpenAL)
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

add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/${AUDIO})

# add_executable(${ENGINE_LIB}
#     ${ENGINE_SRC_FILES}
# )

# target_link_libraries(${ENGINE_LIB} PUBLIC 
#     ${AUDIO_LIB}
# )

# target_include_directories(${ENGINE_LIB} PUBLIC 
#     ${CMAKE_SOURCE_DIR}/Audio_Systems/General/
#     ${CMAKE_SOURCE_DIR}/Audio_Systems/${AUDIO_SYSTEM}/Info/
# )

# target_link_libraries(${ENGINE_LIB} ${TARGET})
