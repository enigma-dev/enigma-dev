SOURCES +=  $(wildcard Universal_System/Extensions/ttf/*.cpp)
override CXXFLAGS += $(shell pkg-config --cflags freetype2)
override LDLIBS += $(shell pkg-config --libs --static freetype2) -lfreetype
