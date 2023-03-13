set(WIDGETS_SOURCES
    gtk_impl.cpp
    widgets.cpp
    dialogs.cpp
)

add_library("WIDGETS_${WIDGETS}" "${WIDGETS_SOURCES}")

find_package(GTK2 2.6 REQUIRED gtk)
target_include_directories("WIDGETS_${WIDGETS}" "${GTK2_INCLUDE_DIRS}")
target_link_libraries("WIDGETS_${WIDGETS}" "${GTK2_LIBRARIES}")
