SOURCES += $(wildcard Platforms/SDL/*.cpp) $(wildcard Platforms/General/POSIX/*.cpp)
override CXXFLAGS += $(shell pkg-config --cflags sdl2) -DENIGMA_WS_SDL

ifeq ($(TARGET-PLATFORM), Windows)
override LDLIBS += -lz -lpthread $(shell pkg-config --libs sdl2) -limagehlp -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid
else
override LDLIBS += -lz -lpthread $(shell pkg-config --libs sdl2)
endif
