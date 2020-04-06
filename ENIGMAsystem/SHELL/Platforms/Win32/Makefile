SOURCES += $(wildcard Platforms/Win32/*.cpp)
SOURCES += Platforms/General/PFshell/PFshell.cpp
override CXXFLAGS += -DENIGMA_PLATFORM_WINDOWS $(shell pkg-config --cflags libffi)
override LDLIBS += -lffi -lcomdlg32 -lgdi32 -lwinmm -lwininet
