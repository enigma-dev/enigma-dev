cmake_minimum_required(VERSION 3.14)

set(ENGINE_LIB "ENGINE")
set(ENGINE_LIB_DESCRIPTION "Enigma Engine")
set(VERSION "1.0.0")

set( CMAKE_CXX_COMPILER "C:\\msys64\\mingw64\\bin\\g++" )
set( CMAKE_C_COMPILER "C:\\msys64\\mingw64\\bin\\gcc" )

set(ENGINE_SRC_FILES 
    *.cpp
)

project(${ENGINE_LIB} VERSION ${VERSION} DESCRIPTION ${ENGINE_LIB_DESCRIPTION})

# for headers needed for Direct Sound system
include_directories(Audio_Systems/DirectSound/Info)
include_directories(Audio_Systems/General/)
include_directories(Audio_Systems/)
include_directories(Universal_System/)

# for headers needed for OpenAL system

# for headers needed for SDL system

# for headers needed for XAudio2 system

# for headers needed if there's no system
add_subdirectory(${CMAKE_SOURCE_DIR}/Audio_Systems/)

add_executable(${ENGINE_LIB}
    ${ENGINE_SRC_FILES}
)

target_link_libraries(${ENGINE_LIB} PUBLIC 
    ${AUDIO_LIB}
)

target_include_directories(${ENGINE_LIB} PUBLIC 
    ${CMAKE_SOURCE_DIR}/Audio_Systems/General/
    ${CMAKE_SOURCE_DIR}/Audio_Systems/${AUDIO_SYSTEM}/Info/
)

#target_link_libraries(${ENGINE_LIB} ${TARGET})
