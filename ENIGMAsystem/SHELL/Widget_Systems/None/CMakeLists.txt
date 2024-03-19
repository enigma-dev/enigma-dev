set(WIDGETS_SYS_NONE_LIB "widgets_sys_none_lib")
set(WIDGETS_SYS_NONE_LIB_DESCRIPTION "Universal System None")

project(${WIDGETS_SYS_NONE_LIB} DESCRIPTION ${WIDGETS_SYS_NONE_LIB_DESCRIPTION})

file(GLOB_RECURSE WIDGETS_SYS_NONE_LIB_SOURCES 
    ../None/nowidget_impl.cpp)

add_library(${WIDGETS_SYS_NONE_LIB} ${WIDGETS_SYS_NONE_LIB_SOURCES})

# add cpps depending on current target
