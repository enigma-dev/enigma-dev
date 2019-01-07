# Universal Sources
set(UNIVERSAL_SOURCES
    ../libEGMstd.cpp
    var4.cpp
    debugscope.cpp
    var4_lua.cpp
    loading.cpp
    rectpack.cpp
    gif_format.cpp
    callbacks_events.cpp
    multifunction_variant.cpp
    shaderstruct.cpp
    spritestruct.cpp
    move_functions.cpp
    estring.cpp
    timelines_object.cpp
    fontinit.cpp
    instance_planar.cpp
    ENIGMA_GLOBALS.cpp
    var_array.cpp
    lives.cpp
    backgroundstruct.cpp
    resource_data.cpp
    reflexive_types.cpp
    depth_draw.cpp
    fileio.cpp
    spriteinit.cpp
    highscore_functions.cpp
    image_formats.cpp
    soundinit.cpp
    instance.cpp
    texture_atlas.cpp
    terminal_io.cpp
    backgroundinit.cpp
    collisions_object.cpp
    globalupdate.cpp
    roomsystem.cpp
    bufferstruct.cpp
    random.cpp
    instance_system.cpp
    fontstruct.cpp
    object.cpp
    mathnc.cpp
    graphics_object.cpp
    zlib.cpp
    transform_object.cpp
    planar_object.cpp
    dynamic_args.cpp
)

add_library("UNIVERSAL_SYSTEM" "${UNIVERSAL_SOURCES}")

# zlib
find_package(ZLIB REQUIRED)
include_directories("${ZLIB_INCLUDE_DIRS}")
target_link_libraries("UNIVERSAL_SYSTEM" "${ZLIB_LIBRARY_RELEASE}")

# Shared Sources
add_subdirectory("../${ENIGMA_DIR}/shared/lodepng" lodepng)
target_link_libraries("UNIVERSAL_SYSTEM" "lodepng")
