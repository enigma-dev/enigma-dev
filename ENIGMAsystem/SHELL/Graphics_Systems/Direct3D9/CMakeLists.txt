set(GRAPHICS_SOURCES
    DX9shader.cpp
    DX9d3d.cpp
    DX9sprite.cpp
    DX9blend.cpp
    DX9draw.cpp
    DX9screen.cpp
    DX9colors.cpp
    DX9background.cpp
    DX9vertex.cpp
    DX9surface.cpp
    DIRECTX9Std.cpp
    DX9textures.cpp
)

add_library("GRAPHICS_${GRAPHICS}" "${GRAPHICS_SOURCES}")

find_package(DirectX REQUIRED)
target_include_directories("GRAPHICS_${GRAPHICS}" PRIVATE "${DirectX9_INCLUDE_DIR}")
target_link_libraries("GRAPHICS_${GRAPHICS}" "${DirectX9_LIBRARIES}")
