############################################################################
# Copyright (C) 2023-2024 Saif Kandil (k0T0z)
#
# This file is a part of the ENIGMA Development Environment.
#
# ENIGMA is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, version 3 of the license or any later version.
#
# This application and its source code is distributed AS-IS, WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along
# with this code. If not, see <http://www.gnu.org/licenses/>
############################################################################

############################################################################
# Note that this file isn't tested yet as ENIGMA's engine still works with
# the old build system. Ignore this file for now.
############################################################################

add_library(steamapi STATIC IMPORTED GLOBAL 
    redistributable_bin/steam_api.lib
)

set_property(TARGET steamapi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(steamapi PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
        IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/game_client/Steamv157/sdk/redistributable_bin/steam_api.lib")

target_link_libraries(steamworks 
    steamapi
)
