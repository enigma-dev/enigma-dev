ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Platforms/SDL/*.cpp) Platforms/General/POSIX/Utility.cpp
override CXXFLAGS += $(shell $(PKG-CONFIG) --cflags sdl2) -DENIGMA_PLATFORM_SDL

ifneq ($(TARGET-PLATFORM), Android)
	SOURCES += Platforms/General/POSIX/shell.cpp
endif

ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += $(wildcard Bridges/SDL-Win32/*.cpp) Platforms/Win32/WINDOWSfilemanip.cpp
override LDLIBS += $(shell $(PKG-CONFIG) --libs --static sdl2)
else
	SOURCES += Platforms/General/POSIX/POSIXfilemanip.cpp
	override LDLIBS += -lpthread $(shell $(PKG-CONFIG) --libs sdl2)
endif
