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

target_link_libraries("GRAPHICS_${GRAPHICS}" d3d9 dxerr9 d3dx9)
