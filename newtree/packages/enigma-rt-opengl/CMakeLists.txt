
project (ENIGMA_RT_OPENGL)

include_directories ("${ENIGMA_RT_SOURCE_DIR}/include")

set (ENIGMA_RT_OPENGL_SRCS
    test.cpp
)

add_library (enigma-rt-opengl ${ENIGMA_RT_OPENGL_SRCS})
