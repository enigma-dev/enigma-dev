cmake_minimum_required(VERSION 3.11)

set(TARGET "test-runner")
set(TARGET_DESCRIPTION "ENIGMA Testing Apparatus")

project(${TARGET})

file(GLOB HARNESS_SOURCES *.cpp)
file(GLOB TEST_SOURCES Tests/*.cpp)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(WIN32)
  add_executable(${TARGET} WIN32 ${HARNESS_SOURCES} ${TEST_SOURCES} "Platform/TestHarness-Win32.cpp")
else()
  add_executable(${TARGET} ${HARNESS_SOURCES} ${TEST_SOURCES} "Platform/TestHarness-X11.cpp")
endif()

include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${GTEST_BOTH_LIBRARIES})

# Find Boost
find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
target_link_libraries(${TARGET} PRIVATE Boost::program_options Boost::filesystem)

if (UNIX)
  find_package(X11 REQUIRED)
  include_directories(${X11_INCLUDE_DIR})
  target_link_libraries(${TARGET} PRIVATE ${X11_LIBRARIES})
endif()

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

install(TARGETS ${TARGET} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.ilk" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
