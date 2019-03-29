cmake_minimum_required(VERSION 3.11)

set(TARGET "emake")
set(TARGET_DESCRIPTION "ENIGMA CLI")

project(${TARGET})

file(GLOB EMAKE_SOURCES *.cpp)

set(SHARED_SOURCES
   "${ENIGMA_DIR}/shared/eyaml/eyaml.cpp"
   "${ENIGMA_DIR}/shared/event_reader/event_parser.cpp"
   "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp"
)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(WIN32)
  add_executable(${TARGET} WIN32 ${EMAKE_SOURCES} ${SHARED_SOURCES})
else()
  add_executable(${TARGET} ${EMAKE_SOURCES} ${SHARED_SOURCES})
endif(WIN32)

include_directories(${TARGET} PRIVATE 
                    "${CMAKE_CURRENT_SOURCE_DIR}"
                    "${ENIGMA_DIR}/shared"
                    "${ENIGMA_DIR}/shared/libpng-util/"
                    "${ENIGMA_DIR}/CompilerSource"
                    "${ENIGMA_DIR}/shared/protos/"
                    "${ENIGMA_DIR}/shared/protos/codegen/"
)

include("${ENIGMA_DIR}/shared/FindEGMDeps.cmake")

if(NOT MSVC)
  # libdl
  target_link_libraries(${TARGET} PRIVATE dl)
endif()

# Find Boost
if(USE_STATIC_LIBS)
  set(Boost_USE_STATIC_LIBS ON) # only find static libs
endif()

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

include("${ENIGMA_DIR}/shared/FindEGMDeps.cmake")

if(WIN32)
  target_link_libraries(${TARGET} PRIVATE Ws2_32 Wldap32 Crypt32 Winmm Userenv Netapi32 version Dwmapi Imm32)
endif()

find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
target_link_libraries(${TARGET} PRIVATE Boost::program_options Boost::filesystem)

install(TARGETS ${TARGET} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.ilk" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
