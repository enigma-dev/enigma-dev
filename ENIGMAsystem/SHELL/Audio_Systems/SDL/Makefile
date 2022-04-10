ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lvorbisfile -lvorbis -lopusfile -lopus -logg -lFLAC -lsndfile -lmodplug -lmpg123 -lssp -lole32 -lwinmm -lshlwapi -fstack-protector
endif

SOURCES += $(wildcard Audio_Systems/SDL/*.cpp)

override CXXFLAGS += $(shell pkg-config --cflags SDL2_mixer)
ifeq  ($(TARGET-PLATFORM), Windows)
	override LDLIBS += $(shell pkg-config --libs --static SDL2_mixer)
else ifeq  ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += /opt/local/lib/libSDL2_mixer.a /opt/local/lib/libogg.a /opt/local/lib/libvorbis.a /opt/local/lib/libvorbisenc.a /opt/local/lib/libvorbisfile.a /opt/local/lib/libFLAC.a /opt/local/lib/libopus.a /opt/local/lib/libopusfile.a /opt/local/lib/libdumb.a /opt/local/lib/libmodplug.a /opt/local/lib/libmpg123.a /opt/local/lib/libfluidsynth.a /opt/local/lib/libsndfile.a
else ifeq  ($(TARGET-PLATFORM), Linux)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbis -lopusfile -lopus -logg -lFLAC -lsndfile -lmodplug -lmpg123 -lssp
else ifeq  ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += /usr/local/lib/libSDL2_mixer.a /usr/local/lib/libogg.a /usr/local/lib/libvorbis.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libFLAC.a /usr/local/lib/libopus.a /usr/local/lib/libopusfile.a /usr/local/lib/libdumb.a -lmodplug -lmpg123 -lfluidsynth -lsndfile -lssp
else ifeq  ($(TARGET-PLATFORM), DragonFlyBSD)
	override LDLIBS += /usr/local/lib/libSDL2_mixer.a /usr/local/lib/libogg.a /usr/local/lib/libvorbis.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libFLAC.a /usr/local/lib/libopus.a /usr/local/lib/libopusfile.a /usr/local/lib/libdumb.a -lmodplug -lmpg123 -lfluidsynth -lsndfile -lssp
else ifeq  ($(TARGET-PLATFORM), OpenBSD)
	override LDLIBS += /usr/local/lib/libSDL2_mixer.a /usr/local/lib/libogg.a /usr/local/lib/libvorbis.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libFLAC.a /usr/local/lib/libopus.a /usr/local/lib/libopusfile.a /usr/local/lib/libdumb.a -lmodplug -lmpg123 -lfluidsynth -lsndfile -lssp
endif
