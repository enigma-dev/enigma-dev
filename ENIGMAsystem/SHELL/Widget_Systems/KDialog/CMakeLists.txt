set(WIDGETS_SOURCES
   dialogs.cpp
)

add_subdirectory("../General/tinyfiledialogs/" "WIDGETS_tinyfiledialogs")
add_library("WIDGETS_${WIDGETS}" "${WIDGETS_SOURCES}")
target_link_libraries("WIDGETS_${WIDGETS}" "WIDGETS_tinyfiledialogs")
