# - Try to find modplug
# Once done this will define
#
# MODPLUG_FOUND - system has libmodplug
# MODPLUG_INCLUDE_DIRS - the libmodplug include directory
# MODPLUG_LIBRARIES - The libmodplug libraries

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules (MODPLUG libmodplug)
  list(APPEND MODPLUG_INCLUDE_DIRS ${MODPLUG_INCLUDEDIR})
endif()

if(NOT MODPLUG_FOUND)
  find_path(MODPLUG_INCLUDE_DIRS libmodplug/modplug.h)
  find_library(MODPLUG_LIBRARIES NAMES modplug)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Modplug DEFAULT_MSG MODPLUG_INCLUDE_DIRS MODPLUG_LIBRARIES)

mark_as_advanced(MODPLUG_INCLUDE_DIRS MODPLUG_LIBRARIES)
