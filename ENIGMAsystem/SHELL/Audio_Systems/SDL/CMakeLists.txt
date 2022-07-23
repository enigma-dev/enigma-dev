cmake_minimum_required(VERSION 3.14)

include_directories(Info/)

set(TARGET_LIB "SDL")

add_library(${TARGET_LIB} 
SDLadvanced.cpp
SDLbasic.cpp
SDLsystem.cpp
)



