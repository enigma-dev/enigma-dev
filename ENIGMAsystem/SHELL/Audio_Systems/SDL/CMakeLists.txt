if(NOT DEFINED PKG-CONFIG)
  set(PKG-CONFIG "pkg-config")
endif()

set(SDL_AUDIO_SYS_LIB "sdl_audio_sys_lib")
set(SDL_AUDIO_SYS_LIB_DESCRIPTION "SDL Audio System")

project(${SDL_AUDIO_SYS_LIB} DESCRIPTION ${SDL_AUDIO_SYS_LIB_DESCRIPTION})

file(GLOB_RECURSE SDL_AUDIO_SYS_LIB_SOURCES 
    *.cpp)

# todo: review makefile again

add_library(${SDL_AUDIO_SYS_LIB} ${SDL_AUDIO_SYS_LIB_SOURCES})
