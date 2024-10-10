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

STEAMWORKS_API_VERSION := 158a

# Directories
STEAM_SDK_PATH := $(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/Steamv${STEAMWORKS_API_VERSION}/sdk

SOURCES += $(wildcard Universal_System/Extensions/Steamworks/*.cpp) \
				$(wildcard Universal_System/Extensions/Steamworks/gameclient/*.cpp) \
                    $(wildcard Universal_System/Extensions/Steamworks/gameclient/utils/*.cpp) \
						$(wildcard Universal_System/Extensions/Steamworks/gameclient/steambinder/*.cpp)

# TODO: This macro won't help as the developer will have to run LGM twice in order to set the macro.
override CXXFLAGS += -I$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/Steamv${STEAMWORKS_API_VERSION}/sdk/public/ \
						-DENIGMA_STEAMWORKS_EXTENSION_ROOT=\"$(abspath .)/Universal_System/Extensions/Steamworks\" \
						-DSTEAMWORKS_API_VERSION=\"$(STEAMWORKS_API_VERSION)\"

include Universal_System/Extensions/Steamworks/gameclient/steambinder/SteamFake/sdk/public/steam/Makefile
