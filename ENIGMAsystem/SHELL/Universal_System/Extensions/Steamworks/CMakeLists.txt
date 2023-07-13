cmake_minimum_required(VERSION 3.5)

project(steamworks VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES 
    main.cpp
)

add_library(steamapi STATIC IMPORTED GLOBAL 
    redistributable_bin/steam_api.lib
)

set_property(TARGET steamapi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(steamapi PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
        IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/redistributable_bin/steam_api.lib")

add_executable(steamworks 
    ${PROJECT_SOURCES}
)

target_link_libraries(steamworks 
    steamapi
)
