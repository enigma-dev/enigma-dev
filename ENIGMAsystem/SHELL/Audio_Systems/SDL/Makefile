ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard Audio_Systems/SDL/*.cpp)

override CXXFLAGS += $(shell pkg-config --cflags SDL2_mixer)
override LDLIBS += $(shell pkg-config --libs SDL2_mixer)
