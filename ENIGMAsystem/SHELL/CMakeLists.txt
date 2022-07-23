cmake_minimum_required(VERSION 3.14)

set(ENGINE_LIB "ENGINE")
set(ENGINE_LIB_DESCRIPTION "Enigma Engine")
set(VERSION "1.0.0")


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
add_subdirectory(Audio_Systems/)




#target_link_libraries(${ENGINE_LIB} ${TARGET})