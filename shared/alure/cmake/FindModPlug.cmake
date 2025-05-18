# - Find ModPlug
# Find the native ModPlug includes and library
#
#  MODPLUG_INCLUDE_DIR - where to find dumb.h
#  MODPLUG_LIBRARIES   - List of libraries when using dumb
#  MODPLUG_FOUND       - True if dumb found.

IF(MODPLUG_INCLUDE_DIR AND MODPLUG_LIBRARIES)
  # Already in cache, be silent
  SET(ModPlug_FIND_QUIETLY TRUE)
ENDIF(MODPLUG_INCLUDE_DIR AND MODPLUG_LIBRARIES)

FIND_PATH(MODPLUG_INCLUDE_DIR libmodplug/modplug.h
          PATHS "${MODPLUG_DIR}"
          PATH_SUFFIXES include
          )

FIND_LIBRARY(MODPLUG_LIBRARIES NAMES modplug
             PATHS "${MODPLUG_DIR}"
             PATH_SUFFIXES lib
             )

MARK_AS_ADVANCED(MODPLUG_LIBRARIES MODPLUG_INCLUDE_DIR)

# handle the QUIETLY and REQUIRED arguments and set MODPLUG_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ModPlug DEFAULT_MSG MODPLUG_LIBRARIES MODPLUG_INCLUDE_DIR)
