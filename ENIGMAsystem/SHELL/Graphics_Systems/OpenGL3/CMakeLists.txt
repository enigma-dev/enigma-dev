set(GRAPHICS_SOURCES
    GL3surface.cpp
    GL3profiler.cpp
    GL3blend.cpp
    GL3shader.cpp
    GL3background.cpp
    GL3colors.cpp
    GL3stdraw.cpp
    OPENGL3Std.cpp
    GL3aux.cpp
    GL3d3d.cpp
    GL3vertex.cpp
    GL3textures.cpp
    GL3screen.cpp
    GL3sprite.cpp
    GL3TextureStruct.cpp
)

add_library("GRAPHICS_${GRAPHICS}" "${GRAPHICS_SOURCES}")

find_package(GLEW REQUIRED)
target_include_directories("GRAPHICS_${GRAPHICS}" PRIVATE "${GLEW_INCLUDE_DIRS}")
target_link_libraries("GRAPHICS_${GRAPHICS}" "${GLEW_LIBRARIES}")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_include_directories("GRAPHICS_${GRAPHICS}" PRIVATE "${OPENGL_INCLUDE_DIRS}")
target_link_libraries("GRAPHICS_${GRAPHICS}" "${OPENGL_LIBRARIES}")
