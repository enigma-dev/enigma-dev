set(GRAPHICS_SOURCES
    GLtextures.cpp
    GLblend.cpp
    GLd3d.cpp
    GLshader.cpp
    GLvertex.cpp
    OPENGLStd.cpp
    GLsprite.cpp
    GLsurface.cpp
    GLbackground.cpp
    GLstdraw.cpp
    GLscreen.cpp
    GLcolors.cpp
)

add_library("GRAPHICS_${GRAPHICS}" "${GRAPHICS_SOURCES}")

find_package(GLEW REQUIRED)
target_include_directories("GRAPHICS_${GRAPHICS}" PRIVATE "${GLEW_INCLUDE_DIRS}")
target_link_libraries("GRAPHICS_${GRAPHICS}" "${GLEW_LIBRARIES}")

find_package(OpenGL REQUIRED)
target_include_directories("GRAPHICS_${GRAPHICS}" PRIVATE "${OPENGL_INCLUDE_DIRS}")
target_link_libraries("GRAPHICS_${GRAPHICS}" "${OPENGL_LIBRARIES}")
