ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lvorbisfile -lvorbis -lssp -lopusfile -lopus -logg -lFLAC -lsndfile -lmodplug -lmpg123 -lole32 -lwinmm -lshlwapi -fstack-protector
endif

SOURCES += $(wildcard Audio_Systems/SDL/*.cpp)

override CXXFLAGS += $(shell pkg-config --cflags SDL2_mixer)
override LDLIBS += $(shell pkg-config --libs SDL2_mixer)
