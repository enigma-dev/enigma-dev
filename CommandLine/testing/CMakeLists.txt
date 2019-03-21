cmake_minimum_required(VERSION 3.11)

set(EXE "test-runner")
set(EXE_DESCRIPTION "ENIGMA Testing Apparatus")

project(${EXE})

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)

file(GLOB HARNESS_SOURCES *.cpp)
file(GLOB TEST_SOURCES Tests/*.cpp)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(WIN32)
  add_executable(${EXE} WIN32 ${HARNESS_SOURCES} ${TEST_SOURCES} "Platform/TestHarness-Win32.cpp")
else()
  add_executable(${EXE} ${HARNESS_SOURCES} ${TEST_SOURCES} "Platform/TestHarness-X11.cpp")
endif()

include_directories(${EXE} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(${EXE} PRIVATE ${GTEST_BOTH_LIBRARIES})

# Find Boost
find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
target_link_libraries(${EXE} PRIVATE Boost::program_options Boost::filesystem)

if (UNIX)
  find_package(X11 REQUIRED)
  include_directories(${X11_INCLUDE_DIR})
  target_link_libraries(${EXE} PRIVATE ${X11_LIBRARIES})
endif()

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

install(TARGETS ${EXE} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.ilk" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
