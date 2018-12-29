set(COLLISION_SOURCES
    placeholderlinks.cpp
    BBOXimpl.cpp
    BBOXfuncs.cpp
    BBOXutil.cpp
)

add_library("COLLISION_${COLLISION}" "${COLLISION_SOURCES}")
