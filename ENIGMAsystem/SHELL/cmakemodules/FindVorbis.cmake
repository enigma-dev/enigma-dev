# - FindVorbis.cmake
# Find the native vorbis includes and libraries
#
# VORBIS_INCLUDE_DIRS - where to find vorbis/vorbis.h, etc.
# VORBIS_LIBRARIES - List of libraries when using vorbis(file).
# VORBIS_FOUND - True if vorbis found.

if(VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY AND VORBISFILE_LIBRARY)
    # Already in cache, be silent
    set(VORBIS_FIND_QUIETLY TRUE)
endif(VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY AND VORBISFILE_LIBRARY)

find_path(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h)

# MSVC built vorbis may be named vorbis_static
# The provided project files name the library with the lib prefix.
find_library(VORBIS_LIBRARY
    NAMES vorbis vorbis_static libvorbis libvorbis_static
)
find_library(VORBISFILE_LIBRARY
    NAMES vorbisfile vorbisfile_static libvorbisfile libvorbisfile_static
)

# Handle the QUIETLY and REQUIRED arguments and set VORBIS_FOUND
# to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VORBIS DEFAULT_MSG
    VORBISFILE_LIBRARY VORBIS_LIBRARY VORBIS_INCLUDE_DIR
)

if(VORBIS_FOUND)
    set(VORBIS_LIBRARIES ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY})
    set(VORBIS_INCLUDE_DIRS ${VORBIS_INCLUDE_DIR})
endif(VORBIS_FOUND)
