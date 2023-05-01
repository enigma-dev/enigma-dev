ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += $(wildcard Platforms/SDL/Win32/*.cpp)
	LDFLAGS += -lwinmm -luser32
else
	ifeq ($(TARGET-PLATFORM), MacOSX)
		SOURCES += $(wildcard Platforms/SDL/MacOSX/*.cpp) 
		override LDLIBS += -lobjc -lObjC++
	else ifeq ($(TARGET-PLATFORM), Linux)
		SOURCES += $(wildcard Platforms/SDL/xlib/*.cpp) 
	else ifeq ($(TARGET-PLATFORM), FreeBSD)
		SOURCES += $(wildcard Platforms/SDL/xlib/*.cpp) 
	else ifeq ($(TARGET-PLATFORM), DragonFlyBSD)
		SOURCES += $(wildcard Platforms/SDL/xlib/*.cpp) 
	else ifeq ($(TARGET-PLATFORM), OpenBSD)
		SOURCES += $(wildcard Platforms/SDL/xlib/*.cpp) 
	endif
endif
SOURCES += $(wildcard Platforms/SDL/*.cpp) 
SOURCES += Platforms/SDL/Desktop/Window.cpp
SOURCES += Platforms/General/POSIX/Utility.cpp
override CFLAGS += -DENIGMA_PLATFORM_SDL
override CXXFLAGS += -DENIGMA_PLATFORM_SDL
