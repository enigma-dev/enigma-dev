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

if(MSVC)
    find_package(DirectX REQUIRED)
    target_include_directories("GRAPHICS_${GRAPHICS}" PRIVATE "${DirectX9_INCLUDE_DIR}")
    message("${DirectX9_LIBRARIES}")
    target_link_libraries("GRAPHICS_${GRAPHICS}" "${DirectX9_LIBRARY}")
else()
    target_link_libraries("GRAPHICS_${GRAPHICS}" d3d9 dxerr9 d3dx9)
endif(MSVC)
