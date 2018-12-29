set(GRAPHICS_SOURCES
    DX11sprite.cpp
    DIRECTX11Std.cpp
    DX11draw.cpp
    DX11shader.cpp
    DX11d3d.cpp
    DX11colors.cpp
    DX11background.cpp
    DX11surface.cpp
    DX11textures.cpp
    DX11blend.cpp
    DX11screen.cpp
    DX11vertex.cpp
)

add_library("GRAPHICS_${GRAPHICS}" "${GRAPHICS_SOURCES}")

find_package(DirectX11 REQUIRED)
target_include_directories("GRAPHICS_${GRAPHICS}" PRIVATE "${DirectX11_INCLUDE_DIRS}")
target_link_libraries("GRAPHICS_${GRAPHICS}" "${DirectX11_LIBRARIES}")
