set(GRAPHICS_BRIDGE_SOURCES
   graphics_bridge.cpp
)

add_library("${PLATFORM}-OpenGL-Bridge" "${GRAPHICS_BRIDGE_SOURCES}")
