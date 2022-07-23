cmake_minimum_required(VERSION 3.14)

target_include_directories(Info/)

set(TARGET_LIB "open_AL")

message("this is ${TARGET_LIB}")

add_library(${TARGET_LIB} 
    *.cpp
    *.h
)



