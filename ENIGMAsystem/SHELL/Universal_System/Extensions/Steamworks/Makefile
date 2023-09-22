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

SOURCES += $(wildcard Universal_System/Extensions/Steamworks/*.cpp) \
				$(wildcard Universal_System/Extensions/Steamworks/gameclient/*.cpp) \
                    $(wildcard Universal_System/Extensions/Steamworks/gameclient/utils/*.cpp) \
						$(wildcard Universal_System/Extensions/Steamworks/gameclient/Binder/*.cpp)

# defining directory of steam header files
# override CXXFLAGS += -I$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/Binder/Steamv157/sdk/public/

SOURCES += $(wildcard Universal_System/Extensions/Steamworks/gameclient/Binder/SteamFake/sdk/public/steam/*.cpp)
override CXXFLAGS += -I$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/Binder/Steamv157/sdk/public/

# export STEAM_SDK_PATH=$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/Binder/Steamv157/sdk/

# ifeq ($(TARGET-PLATFORM), Windows)

#     # defining directory of 64-bit lib file
#     override LDFLAGS += -L$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/Binder/Steamv157/sdk/redistributable_bin/win64/

#     # check https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html#:~:text=Static%20Library%20vs.%20Shared%20Library
#     # adding 64-bit lib file
#     # this is static linking (maybe)
#     override LDLIBS += -lsteam_api64

# else ifeq ($(TARGET-PLATFORM), Linux)

#     # defining directory of 64-bit lib file
#     override LDFLAGS += -L$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/Binder/Steamv157/sdk/redistributable_bin/linux64

#     # check https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html#:~:text=Static%20Library%20vs.%20Shared%20Library
#     # adding 64-bit lib file
#     override LDLIBS += -lsteam_api

# endif
