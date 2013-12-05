
project (ENIGMA_RT CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    LIST(APPEND CMAKE_CXX_FLAGS "-std=c++11 -flto -Wall -Wextra -pedantic -Werror")
endif ()

include_directories ("include")

file (GLOB ENIGMA_RT_SRCS "src/*.cpp")
file (GLOB ENIGMA_RT_HEADERS "include/ert/*.hpp")

add_library (enigma-rt ${ENIGMA_RT_SRCS} ${ENIGMA_RT_HEADERS})
