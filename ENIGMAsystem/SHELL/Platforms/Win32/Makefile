SOURCES += $(wildcard Platforms/Win32/*.cpp)
override CXXFLAGS += -DENIGMA_PLATFORM_WINDOWS $(shell pkg-config --cflags libffi)
override LDLIBS += -lffi -lcomdlg32 -lgdi32 -lgdiplus -lwinmm -lwininet
