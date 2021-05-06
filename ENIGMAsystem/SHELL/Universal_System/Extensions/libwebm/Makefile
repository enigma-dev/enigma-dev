override CXXFLAGS += -I../../shared/libwebm -IUniversal_System/Extensions/libwebm/include -fexceptions -Wno-narrowing
override CFLAGS += -I../../shared/libwebm -IUniversal_System/Extensions/libwebm/include -fexceptions -Wno-narrowing
override LDFLAGS += -lvorbis -logg -lvpx -lz -lc `pkg-config --cflags --libs --static sdl2` -L../../shared/libwebm/ -lwebm -lpthread -lc
SOURCES += $(wildcard Universal_System/Extensions/libwebm/src/*.cpp)
