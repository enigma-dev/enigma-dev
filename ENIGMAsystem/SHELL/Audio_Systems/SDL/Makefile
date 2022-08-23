ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif
SOURCES += $(wildcard Audio_Systems/SDL/*.cpp)
override CXXFLAGS += $(shell pkg-config --cflags SDL2_mixer)
ifeq  ($(TARGET-PLATFORM), Windows)
	override LDLIBS += $(shell pkg-config --libs --static SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp -lwinmm -lole32 -lshlwapi -fstack-protector
else ifeq  ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += /usr/local/lib/libSDL2_mixer.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbis.a /usr/local/lib/libopusfile.a /usr/local/lib/libopus.a /usr/local/lib/libogg.a /usr/local/lib/libFLAC.a /usr/local/lib/libdumb.a /usr/local/lib/libmodplug.a /usr/local/lib/libmpg123.a
else ifeq  ($(TARGET-PLATFORM), Linux)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp
else ifeq  ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp
else ifeq  ($(TARGET-PLATFORM), DragonFlyBSD)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp
else ifeq  ($(TARGET-PLATFORM), OpenBSD)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp
endif
