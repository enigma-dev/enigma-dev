
project (ENIGMA_RT_OPENGL CXX)

if (UNIX)
    LIST(APPEND CMAKE_CXX_FLAGS "-std=c++11 -flto -Wall -Wextra -pedantic -Werror")
endif ()

include_directories ("${ENIGMA_RT_SOURCE_DIR}/include")

file (GLOB ENIGMA_RT_OPENGL_SRCS "src/*.cpp")

add_library (enigma-rt-opengl ${ENIGMA_RT_OPENGL_SRCS})
