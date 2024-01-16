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
	override LDLIBS += -lalure2 -lopenal -lvorbisfile -lvorbisenc -lvorbis -lopusfile -lopus -logg -lFLAC -ldumb -lmodplug -lmpg123 -lsndfile -lfluidsynth

SOURCES += $(wildcard Audio_Systems/OpenAL/*.cpp)
