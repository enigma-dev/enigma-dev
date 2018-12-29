set(NETWORKING_SOURCES
    DPmplay.cpp
)

add_library("NETWORKING_${NETWORKING}" "${NETWORKING_SOURCES}")

if (WIN32)
  target_link_libraries("NETWORKING_${NETWORKING}" "ws2_32")
else()
  target_link_libraries("NETWORKING_${NETWORKING}" "curl")
endif()
