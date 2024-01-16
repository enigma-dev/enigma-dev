ifeq  ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override CXXFLAGS += $(shell pkg-config --cflags openal)
	override LDLIBS += -lALURE32-static -lopenal.dll -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lssp -lwinmm -lole32 -lshlwapi -fstack-protector
else ifeq  ($(TARGET-PLATFORM), MacOSX)
	ifeq ($(shell uname -m), arm64)
		override LDLIBS += /opt/homebrew/lib/libalure-static.a -framework OpenAL /opt/homebrew/lib/libalure-static.a -framework OpenAL /opt/homebrew/lib/libvorbisfile.a /opt/homebrew/lib/libvorbisenc.a /opt/homebrew/lib/libvorbis.a /opt/homebrew/lib/libopusfile.a /opt/homebrew/lib/libopus.a /opt/homebrew/lib/libogg.a /opt/homebrew/lib/libFLAC.a /opt/homebrew/lib/libdumb.a /opt/homebrew/lib/libmodplug.a /opt/homebrew/lib/libmpg123.a
	else
		override LDLIBS += /usr/local/lib/libalure-static.a -framework OpenAL /usr/local/lib/libvorbisfile.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libvorbis.a /usr/local/lib/libopusfile.a /usr/local/lib/libopus.a /usr/local/lib/libogg.a /usr/local/lib/libFLAC.a /usr/local/lib/libdumb.a /usr/local/lib/libmodplug.a /usr/local/lib/libmpg123.a
	endif
else ifeq  ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth
else ifeq  ($(TARGET-PLATFORM), FreeBSD)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), DragonFlyBSD)
	override LDLIBS += -lalure -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth -lssp
else ifeq  ($(TARGET-PLATFORM), OpenBSD)
	override CXXFLAGS += -I../../shared/alure/include
	override LDLIBS += ../../shared/alure/libalure2_s.a -lopenal /usr/local/lib/libvorbisfile.a /usr/local/lib/libvorbis.a /usr/local/lib/libopusfile.a /usr/local/lib/libopus.a /usr/local/lib/libogg.a /usr/lib/libpthread.a /usr/lib/libc++.a /usr/lib/libc++abi.a /usr/lib/libm.a /usr/lib/libsndio.a /usr/local/lib/libvorbisenc.a /usr/local/lib/libFLAC.a /usr/local/lib/libmpg123.a -lsndfile /usr/local/lib/libmp3lame.a
endif

SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)
