ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Platforms/SDL/*.cpp) Platforms/General/POSIX/Utility.cpp
override CXXFLAGS += $(shell $(PKG-CONFIG) --cflags sdl2) -DENIGMA_PLATFORM_SDL

ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += $(wildcard Bridges/SDL-Win32/*.cpp)
override LDLIBS += $(shell $(PKG-CONFIG) --libs --static sdl2)
else
	override LDLIBS += -lpthread $(shell $(PKG-CONFIG) --libs sdl2)
endif
