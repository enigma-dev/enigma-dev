if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
endif(MSVC)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${ENIGMA_DIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

message(STATUS "Initial build flags:")
set(CompilerFlags
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_MINSIZEREL
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )

foreach(CompilerFlag ${CompilerFlags})
  if(MSVC AND USE_STATIC_LIBS)
    # Force static linking of msvc runtime
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endif()
  message(STATUS "  '${CompilerFlag}': ${${CompilerFlag}}")
endforeach()
