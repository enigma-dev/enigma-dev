include Platforms/General/Makefile

SOURCES += $(wildcard Platforms/Win32/*.cpp) Platforms/General/PFshell/PFshell.cpp
override CXXFLAGS += -DENIGMA_PLATFORM_WINDOWS $(shell pkg-config --cflags libffi) -std=c++17
override CFLAGS += -DENIGMA_PLATFORM_WINDOWS $(shell pkg-config --cflags libffi) -std=c++17
override LDFLAGS += -lffi -lcomdlg32 -lgdi32 -lwinmm -lwininet -lshell32 -lole32 -luuid
override LDLIBS += -lffi -lcomdlg32 -lgdi32 -lwinmm -lwininet -lshell32 -lole32 -luuid
