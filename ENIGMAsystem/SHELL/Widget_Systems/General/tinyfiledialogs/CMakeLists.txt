add_subdirectory("../../../../../shared/tinyfiledialogs" "tinyfiledialogs")
add_library("WIDGETS_tinyfiledialogs" "dialogs.cpp")
target_include_directories("WIDGETS_tinyfiledialogs" PRIVATE  "../../../../../shared/")
target_link_libraries("WIDGETS_tinyfiledialogs" "tinyfiledialogs")
