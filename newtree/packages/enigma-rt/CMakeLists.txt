
project (ENIGMA_RT)

include_directories ("include")

set (ENIGMA_RT_SRCS
    variant.cpp
    math.cpp
)

add_library (enigma-rt ${ENIGMA_RT_SRCS})
