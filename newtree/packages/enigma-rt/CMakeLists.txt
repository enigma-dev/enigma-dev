
project (ENIGMA_RT)

include_directories ("include")

set (ENIGMA_RT_SRCS
    variant.cpp
    varargs.cpp
    math.cpp
    vector.cpp
)

set (ENIGMA_RT_HEADERS
	include/ert/buffer.hpp
	include/ert/math.hpp
	include/ert/real.hpp
	include/ert/string.hpp
	include/ert/varargs.hpp
	include/ert/variant.hpp
	include/ert/vector.hpp
)

add_library (enigma-rt ${ENIGMA_RT_SRCS} ${ENIGMA_RT_HEADERS})
