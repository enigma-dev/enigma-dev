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

# Directories
STEAM_SDK_PATH := $(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/Steamv157/sdk
STEAM_FAKE_SDK_PATH := $(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/SteamFake/sdk

LINUX32 := linux32
LINUX64 := linux64
OSX := osx
WIN64 := win64

SOURCES += $(wildcard Universal_System/Extensions/Steamworks/*.cpp) \
				$(wildcard Universal_System/Extensions/Steamworks/gameclient/*.cpp) \
                    $(wildcard Universal_System/Extensions/Steamworks/gameclient/utils/*.cpp) \
						$(wildcard Universal_System/Extensions/Steamworks/gameclient/steambinder/*.cpp)

override CXXFLAGS += -I$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/Steamv157/sdk/public/ \
						-DENIGMA_STEAMWORKS_EXTENSION_ROOT=\"$(abspath .)/Universal_System/Extensions/Steamworks\"
						
FAKE_API_SRC_FILES := $(wildcard Universal_System/Extensions/Steamworks/gameclient/steambinder/SteamFake/sdk/public/steam/*.cpp)

STEAM_FAKE_LIB_NAME := fake_steam_api

STEAM_FAKE_LIB := $(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(LINUX64)/lib$(STEAM_FAKE_LIB_NAME).so

$(STEAM_FAKE_LIB): $(FAKE_API_SRC_FILES)
	@echo "Building fake API..."
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $^

steam_clean:
	rm -rf $(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(LINUX32)/* \
			$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(LINUX64)/* \
			$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(OSX)/* \
			$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(BIN_DIR)/* \
			$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(WIN64)/*

$(shell mkdir -p $(STEAM_FAKE_SDK_PATH)/redistributable_bin \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(LINUX32) \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(LINUX64) \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(OSX) \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(BIN_DIR) \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(WIN64))

# ifeq ($(TARGET-PLATFORM), Windows)

#     # defining directory of 64-bit lib file
#     override LDFLAGS += -L$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/Steamv157/sdk/redistributable_bin/win64/

#     # check https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html#:~:text=Static%20Library%20vs.%20Shared%20Library
#     # adding 64-bit lib file
#     # this is static linking (maybe)
#     override LDLIBS += -lsteam_api64

# else ifeq ($(TARGET-PLATFORM), Linux)

#     # defining directory of 64-bit lib file
#     override LDFLAGS += -L$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/Steamv157/sdk/redistributable_bin/linux64

#     # check https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html#:~:text=Static%20Library%20vs.%20Shared%20Library
#     # adding 64-bit lib file
#     override LDLIBS += -lsteam_api

# endif
