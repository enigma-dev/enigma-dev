set(GRAPHICS_BRIDGE_SOURCES
   graphics_bridge.cpp
)

add_library("${PLATFORM}-${GRAPHICS}-Bridge" "${GRAPHICS_BRIDGE_SOURCES}")
