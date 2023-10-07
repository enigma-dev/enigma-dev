ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif
SOURCES += $(wildcard Audio_Systems/SDL/*.cpp)
override CXXFLAGS += $(shell pkg-config --cflags SDL2_mixer)
ifeq  ($(TARGET-PLATFORM), Windows)
	override LDLIBS += $(shell pkg-config --libs --static SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp -lwinmm -lole32 -lshlwapi -fstack-protector
else ifeq  ($(TARGET-PLATFORM), MacOSX)
	ifeq ($(shell uname -m), arm64)
		override LDLIBS += /opt/homebrew/lib/libSDL2_mixer.a /opt/homebrew/lib/libvorbisfile.a /opt/homebrew/lib/libvorbisenc.a /opt/homebrew/lib/libvorbis.a /opt/homebrew/lib/libopusfile.a /opt/homebrew/lib/libopus.a /opt/homebrew/lib/libogg.a /opt/homebrew/lib/libFLAC.a /opt/homebrew/lib/libdumb.a /opt/homebrew/lib/libmodplug.a /opt/homebrew/lib/libmpg123.a
	else
		override LDLIBS += /usr/local/lib/libSDL2_mixer.a /usr/local/lib/libvorbisfile.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbis.a /usr/local/lib/libopusfile.a /usr/local/lib/libopus.a /usr/local/lib/libogg.a /usr/local/lib/libFLAC.a /usr/local/lib/libdumb.a /usr/local/lib/libmodplug.a /usr/local/lib/libmpg123.a
	endif

else ifeq  ($(TARGET-PLATFORM), Linux)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth
else ifeq  ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), DragonFlyBSD)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), OpenBSD)
	override LDLIBS += $(shell pkg-config --libs SDL2_mixer) -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
endif
