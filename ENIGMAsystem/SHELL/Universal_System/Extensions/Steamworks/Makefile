SOURCES += $(wildcard Universal_System/Extensions/Steamworks/*.cpp) \
				$(wildcard Universal_System/Extensions/Steamworks/game_client/*.cpp)

# defining directory of steam header files
override CXXFLAGS += -I$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/game_client/public/

ifeq ($(TARGET-PLATFORM), Windows)

    # defining directory of 64-bit lib file
    override LDFLAGS += -L$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/game_client/redistributable_bin/win64/

    # check https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html#:~:text=Static%20Library%20vs.%20Shared%20Library
    # adding 64-bit lib file
    # this is static linking (maybe)
    override LDLIBS += -lsteam_api64

else ifeq ($(TARGET-PLATFORM), Linux)

    # defining directory of 64-bit lib file
    override LDFLAGS += -L$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/game_client/redistributable_bin/linux64

    # check https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html#:~:text=Static%20Library%20vs.%20Shared%20Library
    # adding 64-bit lib file
    override LDLIBS += -lsteam_api

endif
