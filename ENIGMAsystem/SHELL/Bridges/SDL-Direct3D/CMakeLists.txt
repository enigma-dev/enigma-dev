set(GRAPHICS_BRIDGE_SOURCES
   graphics_bridge.cpp
)

add_library("${PLATFORM}-Direct3D-Bridge" "${GRAPHICS_BRIDGE_SOURCES}")
