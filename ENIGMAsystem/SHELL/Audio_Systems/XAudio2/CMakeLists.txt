cmake_minimum_required(VERSION 3.14)

include_directories(Info/)

set(TARGET xAudio2)

add_library(${TARGET} 
XAadvanced.cpp
XAbasic.cpp
XAsystem.cpp
)
