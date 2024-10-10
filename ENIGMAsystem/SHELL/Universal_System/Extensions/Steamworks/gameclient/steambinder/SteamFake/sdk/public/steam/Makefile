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

STEAM_FAKE_SDK_PATH := $(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/steambinder/SteamFake/sdk

STEAM_FAKE_SDK_LINUX32 := linux32
STEAM_FAKE_SDK_LINUX64 := linux64
STEAM_FAKE_SDK_OSX := osx
STEAM_FAKE_SDK_WIN64 := win64

FAKE_STEAMWORKS_API_SOURCES := $(wildcard Universal_System/Extensions/Steamworks/gameclient/steambinder/SteamFake/sdk/public/steam/*.cpp)

STEAM_FAKE_LIB_NAME := fake_steam_api

STEAM_FAKE_LIB := $(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_LINUX64)/lib$(STEAM_FAKE_LIB_NAME).so

$(STEAM_FAKE_LIB): $(FAKE_STEAMWORKS_API_SOURCES)
	@echo "Building fake Steamworks API..."
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $^

steam_clean:
	rm -rf $(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_LINUX32)/* \
			$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_LINUX64)/* \
			$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_OSX)/* \
			$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_WIN64)/*

$(shell mkdir -p $(STEAM_FAKE_SDK_PATH)/redistributable_bin \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_LINUX32) \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_LINUX64) \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_OSX) \
					$(STEAM_FAKE_SDK_PATH)/redistributable_bin/$(STEAM_FAKE_SDK_WIN64))

# This is for building the fake API, you can find compile_game target in the SHELL Makefile.
compile_game: $(STEAM_FAKE_LIB)

clean: steam_clean
				