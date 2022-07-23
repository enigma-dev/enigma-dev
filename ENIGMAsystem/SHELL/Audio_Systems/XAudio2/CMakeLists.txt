cmake_minimum_required(VERSION 3.14)

target_include_directories(Info/)

set(TARGET_LIB "x_audio2")

message("this is ${TARGET_LIB}")

# adding XAudio2 as module
add_library(${TARGET_LIB} 
    *.cpp
    *.h
)
