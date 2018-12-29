set(GRAPHICS_SOURCES
    NONEcolors.cpp
    fillin.cpp
    NONEd3d.cpp
    NONEblend.cpp
)

add_library("GRAPHICS_${GRAPHICS}" "${GRAPHICS_SOURCES}")
