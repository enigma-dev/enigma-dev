
project (ENIGMA_RT CXX)

if (UNIX)
    LIST(APPEND CMAKE_CXX_FLAGS "-std=c++11 -flto -Wall -Wextra -pedantic -Werror")
endif ()

include_directories ("include")

file (GLOB ENIGMA_RT_SRCS "src/*.cpp")
file (GLOB ENIGMA_RT_HEADERS "include/*.hpp")

add_library (enigma-rt ${ENIGMA_RT_SRCS} ${ENIGMA_RT_HEADERS})
