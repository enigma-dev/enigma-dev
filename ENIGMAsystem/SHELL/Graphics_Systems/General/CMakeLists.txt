set(GRAPHICS_GENERAL_SOURCES
   GSprimitives.cpp
   GSmodel.cpp
   GSsprite.cpp
   GScurves.cpp
   GSsurface.cpp
   GSstdraw.cpp
   GSbackground.cpp
   GSmatrix.cpp
   GSvertex.cpp
   GStiles.cpp
   GSfont.cpp
   GSscreen.cpp
   GScolors.cpp
)

add_library("GRAPHICS_GENERAL" "${GRAPHICS_GENERAL_SOURCES}")
