ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Platforms/MacOSX/*.cpp) Platforms/General/POSIX/Utility.cpp
SOURCES += Platforms/MacOSX/Cocoa/Window.cpp
SOURCES += Platforms/MacOSX/Desktop/Window.cpp
override CFLAGS += -DENIGMA_PLATFORM_SDL
override CXXFLAGS += -DENIGMA_PLATFORM_SDL
override LDFLAGS += /opt/local/lib/libiconv.a /opt/local/lib/libSDL2.a -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal
override LDLIBS += /opt/local/lib/libiconv.a /opt/local/lib/libSDL2.a -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal
