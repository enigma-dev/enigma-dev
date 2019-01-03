set(WIDGETS_SOURCES
   dialogs.cpp
   menus.cpp
   widgets.cpp
   resources.rc
)

add_library("WIDGETS_${WIDGETS}" "${WIDGETS_SOURCES}")
target_link_libraries("WIDGETS_${WIDGETS}" comctl32 uuid ole32)
