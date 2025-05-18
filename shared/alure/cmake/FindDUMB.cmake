# - Find DUMB
# Find the native DUMB includes and library
#
#  DUMB_INCLUDE_DIR - where to find dumb.h
#  DUMB_LIBRARIES   - List of libraries when using dumb
#  DUMB_FOUND       - True if dumb found.

IF(DUMB_INCLUDE_DIR AND DUMB_LIBRARIES)
  # Already in cache, be silent
  SET(DUMB_FIND_QUIETLY TRUE)
ENDIF(DUMB_INCLUDE_DIR AND DUMB_LIBRARIES)

FIND_PATH(DUMB_INCLUDE_DIR dumb.h
          PATHS "${DUMB_DIR}"
          PATH_SUFFIXES include
          )

FIND_LIBRARY(DUMB_LIBRARIES NAMES dumb
             PATHS "${DUMB_DIR}"
             PATH_SUFFIXES lib
             )

MARK_AS_ADVANCED(DUMB_LIBRARIES DUMB_INCLUDE_DIR)

# handle the QUIETLY and REQUIRED arguments and set DUMB_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DUMB DEFAULT_MSG DUMB_LIBRARIES DUMB_INCLUDE_DIR)
