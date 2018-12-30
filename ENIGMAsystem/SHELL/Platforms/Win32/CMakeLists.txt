set(PLATFORM_SOURCES
   WINDOWScallback.cpp
   WINDOWSexternals.cpp
   WINDOWSfilemanip.cpp
   WINDOWShandle.cpp
   WINDOWSjoystick.cpp
   WINDOWSmain.cpp
   WINDOWSregistry.cpp
   WINDOWSsystem.cpp
   WINDOWSthreads.cpp
   WINDOWSwindow.cpp
)

add_compile_definitions(ENIGMA_PLATFORM_WINDOWS)
add_library("PLATFORM_${PLATFORM}" "${PLATFORM_SOURCES}")

target_link_libraries("PLATFORM_${PLATFORM}" ffi comdlg32 gdi32 winmm wininet)
target_include_directories("PLATFORM_${PLATFORM}" PRIVATE)
