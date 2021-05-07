override CXXFLAGS += `pkg-config sdl2 --cflags` -I../../shared/libwebm -IUniversal_System/Extensions/libwebm/include -fexceptions -Wno-narrowing
override CFLAGS += `pkg-config sdl2 --cflags` -I../../shared/libwebm -IUniversal_System/Extensions/libwebm/include -fexceptions -Wno-narrowing
override LDFLAGS += -lvorbis -logg -lvpx -lz -lc `pkg-config sdl2 --libs --static` -L../../shared/libwebm/ -lwebm
ifeq ($(UNIX_BASED), true)
	ifeq ($(OS), FreeBSD)
		override LDFLAGS += -lc
	endif
	override LDFLAGS += -lpthread
endif
SOURCES += $(wildcard Universal_System/Extensions/libwebm/src/*.cpp)
