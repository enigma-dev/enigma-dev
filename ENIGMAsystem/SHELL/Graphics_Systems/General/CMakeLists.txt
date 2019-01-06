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

find_package(GLM REQUIRED)
target_include_directories("GRAPHICS_GENERAL" PRIVATE "${GLM_INCLUDE_DIRS}")
