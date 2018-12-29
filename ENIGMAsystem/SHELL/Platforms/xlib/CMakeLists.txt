set(PLATFORM_SOURCES
   XLIBwindow.cpp
   LINUXjoystick.cpp
   XLIBsystem.cpp
   XLIBmain.cpp
)

add_library("PLATFORM_${PLATFORM}" "${PLATFORM_SOURCES}")

find_package(X11 REQUIRED)
target_link_libraries("PLATFORM_${PLATFORM}" "${X11_LIBRARIES}")
target_include_directories("PLATFORM_${PLATFORM}" PRIVATE "${X11_INCLUDE_DIR}")
