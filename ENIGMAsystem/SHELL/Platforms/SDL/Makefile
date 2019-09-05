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
	SOURCES += Platforms/General/POSIX/POSIXdirectory.cpp
	override LDLIBS += -lpthread $(shell $(PKG-CONFIG) --libs --static sdl2)
	ifeq ($(TARGET-PLATFORM), Linux)
		SOURCES += $(wildcard Platforms/SDL/xlib/*.cpp)
		SOURCES += Platforms/xlib/XLIBicon.cpp
		override CXXFLAGS += -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
		override CFLAGS += -I/usr/X11R6/include -I/usr/include -I/usr/include/libpng16
		override LDLIBS += -Wl,-Bstatic -lpng16 -lz -Wl,-Bdynamic -lz -lpthread -lX11
		override LDFLAGS += -L/usr/X11R6/lib -L/usr/lib
	endif
endif
