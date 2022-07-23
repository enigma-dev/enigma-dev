cmake_minimum_required(VERSION 3.14)

target_include_directories(Info/)

set(TARGET_LIB "direct_sound")

message("this is ${TARGET_LIB}")

# adding DirectSound as module
add_library(${TARGET_LIB} 
    *.cpp
    *.h
)

